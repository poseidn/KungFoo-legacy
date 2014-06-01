#include "MenuState.h"

#include <DescentEngine/src/EntityEngine/EntityFactory.h>
#include <DescentEngine/src/EntityEngine/SingleVisualEntity.h>
#include <DescentEngine/src/Visuals/TextVisual.h>

#include <DescentLogic/src/Config/GameRules.h>

void MenuState::addMenuItem(MenuItem & item) {
	/*	EntityFactory fact(getEngines());
	 Entity * guyEnt = fact.create<  SingleVisualEntity>("bullet", item.MarkerPos);
	 getEngines().entityEngine().addEntity(guyEnt, &getManagedEntityList());
	 */
	// add text
	Vector2 textPos(item.MarkerPos.x() + 0.9f, item.MarkerPos.y() - 0.5f);

	TexturePtr textTex = getEngines().resourceEngine().loadImage("textChars.png");
	TextVisual * textV = new TextVisual(getEngines().renderEngine().getScreenTransform(), textTex, textPos,
			item.Text);
	getEngines().renderEngine().addTextVisual(textV);
	item.TextVis = textV;

	if (m_currentItem < 0)
		m_currentItem = 0;

	m_menuItems.push_back(item);
}

void MenuState::bootstrapMenu() {
	EntityFactory fact(getEngines());
	Entity * guyEnt = fact.createFromTemplateName<SingleVisualEntity>("bullet", Vector2(0.0f, 0.0f));
	guyEnt->getActiveVisual().get().setIngame(getEngines().renderEngine().getScreenTransform(),
			Vector2(0.0f, 0.0f), false);
	getEngines().entityEngine().addEntity(guyEnt, &getManagedEntityList());

	m_currentItemPointer = guyEnt;
	m_currentItem = -1;
}

size_t MenuState::activeDevices() const {

	int numEnabled = 0;
	for (auto const& it : getDeviceItems()) {
		if (it.m_isEnabled)
			numEnabled++;
	}
	return numEnabled;
}

void MenuState::updateDevices() {

	// is there only one enabled?
	const size_t numEnabled = activeDevices();

	for (auto & it : getDeviceItems()) {
		// show disable only, if there is more than one disabled
		it.m_DisableText->getActiveVisual().get().setVisible(false);
		it.m_EnableText->getActiveVisual().get().setVisible(false);

		if (it.m_isEnabled /*&& (numEnabled > 1)*/) {
			it.m_DisableText->getActiveVisual().get().setVisible(true);
		}
		if (!it.m_isEnabled && (numEnabled <= GameRules::MaxPlayers)) {
			it.m_EnableText->getActiveVisual().get().setVisible(true);
		}

		// set transparency
		it.m_Icon->getActiveVisual().get().setVisible(it.m_isEnabled);
		it.m_IconPassive->getActiveVisual().get().setVisible(!it.m_isEnabled);

	}
}

std::string MenuState::getCurrentItemId() const {

	if (m_currentItem < 0)
		return "";

	return m_menuItems[m_currentItem].Id;
}

void MenuState::moveMenuPointer(MovementDirection dir) {
	int item = m_currentItem;

	if (dir == MovementDirection::Up) {
		item--;
	}
	if (dir == MovementDirection::Down) {
		item++;
	}

// clamp the value into a safe range
	item = std::max(0, item);
	item = std::min(int(m_menuItems.size()) - 1, item);

	setCurrentItem(item);
}

void MenuState::setCurrentItem(int item) {
	m_currentItem = item;
// clamp the value into a safe range
	m_currentItem = std::max(0, m_currentItem);
	m_currentItem = std::min(int(m_menuItems.size()) - 1, m_currentItem);

	MenuItem & thisItem = m_menuItems[m_currentItem];

// move button
	Vector2 posMarker = thisItem.MarkerPos;

	// slightly move down, because the "pointing" element of the fist is in the upper half of
	// the image
	posMarker.setY(posMarker.y() - 0.2f);

	m_currentItemPointer->setMoveIntent(posMarker);
}

