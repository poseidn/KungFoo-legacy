#include "GameTemplates.h"

#include <sstream>

#include <DescentEngine/src/EntityEngine/EntityEngine.h>
#include <DescentLogic/src/Config/PlayerMovement.h>
#include <DescentLogic/src/Config/EnemyBehaviour.h>
#include <DescentLogic/src/Entities/TextureIds.h>

void GameTemplates::registerTemplates(EntityEngine & entNg) {
	const size_t DefaultFrameCount = 1;
	const size_t DefaultFrameCountAnimation = 8;
	const float DefaultFrameTiming = .05f;
	const float WalkAnimationFrameTiming = .04f;

	const float DeviceEnableHeight = 0.8f;
	const float DeviceEnableWidth = 2.7f;

	const float DeviceIconHeight = 2.0f;
	const float DeviceIconWidth = 2.6f;

	const float JumpFrameTiming = PlayerMovement::JumpTime / float(DefaultFrameCountAnimation);
	const float KickFrameTiming = PlayerMovement::KickTime / float(DefaultFrameCountAnimation);
	const float EnemyHitFrameTiming = EnemyBehaviour::HitTime / float(DefaultFrameCountAnimation);

	const Rectangle2 DefaultFighterBoundingBox(0.9, 0.9);
	const Rectangle2 DefaultFighterImageBox(3.6, 3.6);

	auto registerFullTurn = []( EntityTemplate::TextureNames & texConfigs,
			TextureId firstId,
			std::string const& prefixName,
			size_t frameCount,
			float animationTiming ) {

		for ( size_t i = 0; i < 8; i++) {
			std::stringstream fullname;
			fullname << prefixName << "_" << ( i * 45);
			texConfigs.push_back( EntityTemplate::TextureConfig( firstId + i,
							{	fullname.str()},
							frameCount,
							animationTiming ));
		}
	};

	auto registerPlayer =
			[&]( std::string const& name, std::string const& texturePrefix, bool withDead, bool withBiking, bool withHit,
					Rectangle2 const& boundingBox, Rectangle2 const& imageBox ) {
				EntityTemplate::TextureNames texConfigs;
				registerFullTurn ( texConfigs,DescentTextureIds::Walk_0, texturePrefix + "_walk",
						DefaultFrameCountAnimation, WalkAnimationFrameTiming );
				registerFullTurn ( texConfigs,DescentTextureIds::Stand_0, texturePrefix + "_stand",
						DefaultFrameCountAnimation, WalkAnimationFrameTiming );
				registerFullTurn ( texConfigs,DescentTextureIds::Kick_0, texturePrefix + "_kick",
						DefaultFrameCountAnimation, WalkAnimationFrameTiming );
				registerFullTurn ( texConfigs,DescentTextureIds::Jump_0, texturePrefix + "_jump",
						DefaultFrameCountAnimation, WalkAnimationFrameTiming );
				if ( withBiking)
				{	registerFullTurn ( texConfigs,DescentTextureIds::Biking_0, texturePrefix + "_biking",
							DefaultFrameCountAnimation, WalkAnimationFrameTiming );}
				if ( withDead)
				{	registerFullTurn ( texConfigs,DescentTextureIds::Dead_0, texturePrefix + "_dead",
							DefaultFrameCount, WalkAnimationFrameTiming );}
				if ( withHit)
				{	registerFullTurn ( texConfigs,DescentTextureIds::Hit_0, texturePrefix + "_hit",
							DefaultFrameCountAnimation, WalkAnimationFrameTiming );}

				entNg.addTemplate(name, new EntityTemplate( texConfigs, boundingBox,
								imageBox, LayerPriorityTopMost,
								CollisionMode::AlwaysCollide,
								true) );
			};

	auto registerPlayerEqualNames =
			[&] ( std::string const& name, bool withDead, bool withBiking, bool withHit, Rectangle2 const& boundingBox,
					Rectangle2 const& imageBox ) {
				registerPlayer( name, name, withDead, withBiking, withHit, boundingBox, imageBox );
			};

	auto registerStatic = [&] ( std::string const& name,
			Rectangle2 const& boundingBox,
			Rectangle2 const& imageBox,
			LayerPriority prio,
			CollisionMode colMode ) {

		EntityTemplate::TextureConfig texConfig(DescentTextureIds::Dummy,
				{	name},
				DefaultFrameCount );

		entNg.addTemplate(name, new EntityTemplate( {texConfig}, boundingBox,
						imageBox, prio,
						colMode,
						false) );
	};

	auto registerStaticMultiTex = [&] ( std::string const& name,
			std::vector< std::string > const& fileNames,
			Rectangle2 const& boundingBox,
			Rectangle2 const& imageBox,
			LayerPriority prio,
			CollisionMode colMode ) {

		EntityTemplate::TextureConfig texConfig(DescentTextureIds::Dummy,
				fileNames,
				DefaultFrameCount );

		entNg.addTemplate(name, new EntityTemplate( {texConfig}, boundingBox,
						imageBox, prio,
						colMode,
						false) );
	};

	auto registerUiElement = [&] ( std::string const& name,
			Rectangle2 const& boundingBox )
	{
		registerStatic ( name, boundingBox, boundingBox, LayerPriorityTopMost,
				CollisionMode::NeverCollide);
	};

	registerPlayerEqualNames("player1", false, true, false, DefaultFighterBoundingBox, DefaultFighterImageBox);
	registerPlayerEqualNames("player2", false, true, false, DefaultFighterBoundingBox, DefaultFighterImageBox);
	registerPlayerEqualNames("player3", false, true, false, DefaultFighterBoundingBox, DefaultFighterImageBox);
	registerPlayerEqualNames("player4", false, true, false, DefaultFighterBoundingBox, DefaultFighterImageBox);

	// todo: enemy hat ein bissle anderes bounding box
	registerPlayerEqualNames("enemy1", true, false, true, DefaultFighterBoundingBox, { 2.7, 2.7 });

	// walls
	registerStatic("wall_wood_side1", { .25, 2.0 }, { .5, .5 }, LayerPriorityWallsTop,
			CollisionMode::AlwaysCollide);
	registerStatic("wall_wood_front1", { 3.0, 2.0 }, { 3.0, 2.0 }, LayerPriorityWallsBottom,
			CollisionMode::AlwaysCollide);
	registerStatic("wall_wood_top1", { 3.0, 2.0 }, { 3.0, 2.0 }, LayerPriorityWallsBottom,
			CollisionMode::AlwaysCollide);
	registerStatic("wall_wood_top2", { 3.0, 0.5 }, { 3.0, 0.5 }, LayerPriorityWallsBottom,
			CollisionMode::AlwaysCollide);
	registerStatic("wall_paper_front1", { 2.0, 2.0 }, { 2.0, 2.0 }, LayerPriorityWallsBottom,
			CollisionMode::AlwaysCollide);
	registerStatic("wall_paper_front2", { 2.0, 2.0 }, { 2.0, 2.0 }, LayerPriorityWallsBottom,
			CollisionMode::AlwaysCollide);

	// floor
	registerStaticMultiTex("floor_wood2", { "floor_wood2", "floor_wood3", "floor_wood2", "floor_wood3",
			"floor_wood4" }, { 2.0, 2.0 }, { 2.0, 2.0 }, LayerPriorityFloorTiles, CollisionMode::NeverCollide);

	registerStaticMultiTex("floor_gras1", { "gras1" }, { 2.0, 2.0 }, { 2.0, 2.0 }, LayerPriorityFloorTiles,
			CollisionMode::NeverCollide);

	registerStaticMultiTex("floor_water1", { "water1" }, { 2.0, 2.0 }, { 2.0, 2.0 }, LayerPriorityFloorTiles,
			CollisionMode::AlwaysCollide);

	// stuff
	registerStatic("cabinet1", { 2.0, 1.5 }, { 2.0, 1.5 }, LayerPriorityFurnitureTransparent,
			CollisionMode::JumpNoCollide);
	registerStaticMultiTex("vase1", { "vase1", "vase2" }, { 1.0, 2.0 }, { 1.0, 2.0 },
			LayerPriorityFurnitureTransparent, CollisionMode::AlwaysCollide);
	registerStaticMultiTex("carpet1", { "carpet1", "carpet2" }, { 6.0, 6.0 }, { 6.0, 6.0 },
			LayerPriorityFurniture, CollisionMode::NeverCollide);

	// decals
	registerUiElement("blood1", { 0.2, 0.2 });
	registerUiElement("blood2", { 0.15, 0.15 });
	registerUiElement("blood3", { 0.15, 0.15 });

	registerUiElement("tooth1", { 0.3, 0.3 });

	// various overlay stuff
	registerStatic("intro_bg", { 20.0, 20.0 }, { 20.0, 20.0 }, LayerPriorityBottomMinusOne,
			CollisionMode::NeverCollide);
	registerUiElement("intro_guy", { 10.0, 10.0 });
	registerUiElement("bullet", { 1.5, 1.5 });
	registerUiElement("game_logo", { 10.0, 5.0 });

	registerUiElement("intro_logo_kungfoo", { 10.0, 5.0 });
	registerUiElement("intro_logo_maker", { 10.0, 4.6 });
	registerUiElement("intro_logo_fist_left", { 3.0, 5.0 });
	registerUiElement("intro_logo_fist_right", { 3.0, 5.0 });
	registerUiElement("intro_logo_bar", { 12.0, 1.25 });

	registerUiElement("xbox-gamepad-labels", { 4.0, 3.0 });
	registerUiElement("xbox-gamepad", { DeviceIconWidth, DeviceIconHeight });
	registerUiElement("xbox-gamepad-passive", { DeviceIconWidth, DeviceIconHeight });

	registerUiElement("xbox-gamepad-disable", { DeviceEnableWidth, DeviceEnableHeight });
	registerUiElement("xbox-gamepad-enable", { DeviceEnableWidth, DeviceEnableHeight });

	registerUiElement("ouya-gamepad", { DeviceIconWidth, DeviceIconHeight });
	registerUiElement("ouya-gamepad-passive", { DeviceIconWidth, DeviceIconHeight });

	registerUiElement("ouya-gamepad-disable", { DeviceEnableWidth, DeviceEnableHeight });
	registerUiElement("ouya-gamepad-enable", { DeviceEnableWidth, DeviceEnableHeight });

	registerUiElement("keyboard", { DeviceIconWidth, DeviceIconHeight });
	registerUiElement("keyboard-passive", { DeviceIconWidth, DeviceIconHeight });

	registerUiElement("keyboard-disable", { DeviceEnableWidth, DeviceEnableHeight });
	registerUiElement("keyboard-enable", { DeviceEnableWidth, DeviceEnableHeight });

	registerUiElement("player_arrow", { 1.0f, 1.0f });
}

