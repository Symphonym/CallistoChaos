#include "EngineInfo.h"
#include "State.h"

namespace cc
{
	Engine* EngineInfo::m_engine = nullptr;

	sf::Event &EngineInfo::getEvent()
	{
		return m_engine->m_events;
	}
	Renderer &EngineInfo::getRender()
	{
		return m_engine->m_renderer;
	}
	AssetManager &EngineInfo::getAssets()
	{
		return m_engine->m_assets;
	}

	double EngineInfo::getDelta()
	{
		return m_engine->m_deltaTime;
	}
	double EngineInfo::getFps()
	{
		return m_engine->m_fps;
	}

	void EngineInfo::changeState(std::unique_ptr<State> state)
	{
		m_engine->changeState(std::move(state));
	}
	void EngineInfo::pushState(std::unique_ptr<State> state)
	{
		m_engine->pushState(std::move(state));
	}
	void EngineInfo::popState()
	{
		m_engine->popState();
	}
};