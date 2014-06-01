#include "AttackAspect.h"

#include <DescentEngine/src/EntityEngine/EntityFactory.h>
#include <DescentEngine/src/Physics/EntityRigidBodyState.h>

#include "../GameState.h"
#include "../../Entities/GameTemplates.h"
#include "../../Entities/PlayerEntity.h"
#include "../../Entities/EnemyEntity.h"
#include "../../Entities/WallEntity.h"
#include "../../Config/EnemyBehaviour.h"
#include "../../Config/PlayerMovement.h"
#include "../../Config/GameRules.h"
#include "../../Entities/TextureIds.h"
#include "../../Entities/BloodEntity.h"

void AttackAspect::init(GameState & gs) {
	gs.slotStep.subscribe([this] ( GameState & g, float t)
	{	this->step(g,t);});

}

void AttackAspect::step(GameState & gs, float deltaT) {
	// handle attack for all players

	std::list<PlayerEntity*> playerFakeList;

	auto hitEnemy =
			[ &gs ]( PlayerEntity * attacker, EnemyEntity * enemy, PlayerData * player, Vector2 const& conVector ) {
				enemy->getHitAction().activate(EnemyBehaviour::HitTime);
				// move slightly back
				// todo: this kick direction looks a bit strange
				//auto kickBack = conVector * PlayerMovement::HitKickBackFactor;
				//auto kickForce = conVector * 3000.0f;
				//enemy->setMoveDelta(kickBack);
				auto conVectorNorm = conVector.normalizedCopy();
				enemy->setDirection(-conVectorNorm);

				// does not look so good
				//enemy->getRigidBody()->applyCentralImpulse( btVector3( kickForce.x(), kickForce.y(), 0.0f) );
				enemy->activateLastHit();

				EntityFactory fact( gs.getEngines());
				//fact.createPunchParticles( enemy->getPosition(), conVector.normalizedCopy() );
				std::list <BloodEntity *> newEnts = fact.createPunchParticlesTex < BloodEntity > ( enemy->getPosition(), conVector.normalizedCopy() );
				//const float bloodSpeed = 5.0f;
				for ( auto e: newEnts ) {
					gs.addFragment( e );

				}

				logging::Info() << "Enemy player hit";
				if ( attacker->getBikingAction().isActive())
				{
					enemy->subtractLife( 127);
				} else {
					enemy->subtractLife( PlayerMovement::KickSubtractLife );
				}

				logging::Info() << "Enemy has " << enemy->getLife() << " life left";

				if ( enemy->getLife() < 0)
				{
					logging::Info() << "Enemy died";
					enemy->die( );
					enemy->updateVisual( gs.getEngines() );

					gs.slotEnemyDied.signal(gs, player->Id, enemy);
					gs.increaseScore( player->Id, GameRules::PointsForKill );
				}

				if( attacker->getBikingAction().isActive() )
				attacker->getBikingAction().setProcessed();
				if( attacker->getKickAction().isActive() )
				attacker->getKickAction().setProcessed();
			};

	for (PlayerData & pd : gs.getPlayers()) {
		PlayerEntity * pent = pd.Entity;

		if (pent == nullptr)
			continue;

		// check if the player kicks and whether there is an enemy close by
		if ((pent->getKickAction().isActive() && !pent->getKickAction().wasProcessed())
				|| (pent->getBikingAction().isActive() && !pent->getBikingAction().wasProcessed())) {

			attackOnList(pent, gs.getEnemies(), hitEnemy, &pd, PlayerMovement::AttackAngle,
					PlayerMovement::AttackRangeSquared, EnemyBehaviour::HitTime,
					PlayerMovement::KickAffectCount);

		}

		playerFakeList.push_back(pent);
	}

	auto hitOnPlayer =
			[]( EnemyEntity * attacker, PlayerEntity * enemy, PlayerData * player, Vector2 const& conVector ) {
				//enemy->getHitAction().activate(EnemyBehaviour::HitTime);
				// move slightly back
				// todo: this kick direction looks a bit strange
				/*auto kickBack = conVector * 0.25f;
				 enemy->setMoveDelta(kickBack);
				 auto conVectorNorm = conVector.normalizedCopy();
				 enemy->setDirection(-conVectorNorm);*/
			};

	// checks for enemies hitting player
	for (auto * enemy : gs.getEnemies()) {
		if (enemy->getKickAction().isActive() && !enemy->getKickAction().wasProcessed()) {

			attackOnList(enemy, playerFakeList, hitOnPlayer, nullptr, PlayerMovement::AttackAngle,
					PlayerMovement::AttackRangeSquared, EnemyBehaviour::HitTime, 1);
			enemy->getKickAction().setProcessed();
		}
	}

	/*for (auto & enemy : gs.getEnemies()) {
	 if (enemy->getHitAction().isActive())
	 continue;
	 auto conVector = enemy->getPosition() - gs.getPlayerEntity()->getPosition();

	 if (conVector.magSquared() < PlayerMovement::AttackRangeSquared) {
	 // check for correction direction
	 const Vector2 conVec = enemy->getPosition() - gs.getPlayerEntity()->getPosition();
	 const float angle = Vector2::angleBetween(conVec, gs.getPlayerEntity()->getDirection());
	 if (angle < PlayerMovement::AttackAngle) {

	 enemy->getHitAction().activate(EnemyBehaviour::HitTime);
	 // move slightly back

	 // could be made delayed
	 // todo: this kick direction looks a bit strange
	 auto kickBack = conVector * 0.55f;
	 enemy->setMoveDelta(kickBack);
	 auto conVectorNorm = conVector.normalizedCopy();
	 enemy->setDirection(-conVectorNorm);

	 //enemy->changeActiveVisual(gs.getEngines(), DescentTextureIds::Hit_0);
	 }
	 }
	 }*/

}

