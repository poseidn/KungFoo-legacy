#pragma once

#include <DescentEngine/src/CinematicEngine/CinematicEngine.h>
class IntroCinematic {
public:
	static void setup(CinematicEngine & eg) {

		EntityFactory fact(eg.getEngines());
		eg.getEngines().renderEngine().setCameraLocation(Vector3(0.0f, 0.0f, 0.0f));
		auto bgEnt = fact.createFromTemplateName<SingleVisualEntity>("intro_bg", Vector2(0.0f, 10.0f));

		Entity * guyEnt = fact.createFromTemplateName<SingleVisualEntity>("intro_guy", Vector2(0.0f, 0.0f));
		Entity * guyEnt2 = fact.createFromTemplateName<SingleVisualEntity>("intro_guy", Vector2(0.0f, 0.0f));

		eg.getEngines().entityEngine().addEntity(bgEnt);
		eg.getEngines().entityEngine().addEntity(guyEnt, "guy1");
		eg.getEngines().entityEngine().addEntity(guyEnt2, "guy2");

		eg.addTimelineItem(
				std::make_shared < CinematicTransformAnimation
						> ("guy1", Vector2(0, 0), Vector2(5, 5), CinematicTimespan(2, 5)));

		eg.addTimelineItem(
				std::make_shared < CinematicTransformAnimation
						> ("guy2", Vector2(20, 0), Vector2(8, 7), CinematicTimespan(0, 3)));
	}
};
