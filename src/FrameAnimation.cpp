#include "FrameAnimation.h"
#include <stdexcept>
#include "EngineInfo.h"

namespace cc
{
	FrameAnimation::FrameAnimation()
	 :
	 m_frameDuration(0),
	 m_frameIndex(0),
	 m_activeAnimation("")
	{

	}

	void FrameAnimation::createAnimation(const std::string &name)
	{
		std::vector<Frame> frames;
		m_animations[name] = frames;

		// Set this to active animation
		m_activeAnimation = name;
	}
	void FrameAnimation::setAnimation(const std::string &name, sf::Shape &shape)
	{
		m_activeAnimation = name;

		// Set first frame of new animation
		if(m_frameIndex >= m_animations[m_activeAnimation].size())
			m_frameIndex = 0;

		shape.setTextureRect(m_animations[m_activeAnimation][m_frameIndex].frame);
	}
	void FrameAnimation::deleteAnimation(const std::string &name)
	{
		if(m_animations.find(name) != m_animations.end())
			m_animations.erase(m_animations.find(name));
	}

	FrameAnimation &FrameAnimation::pushFrame(const sf::IntRect &frame, double frameDuration)
	{
		if(m_animations.find(m_activeAnimation) != m_animations.end())
		{
			Frame data;
			data.frame = frame;
			data.frameDuration = frameDuration;
			m_animations[m_activeAnimation].push_back(data);

		}
		else
			throw std::runtime_error("The FrameAnimation \"" + m_activeAnimation + "\" could not be found.");

		// Return this, to allow chaining calls
		return *this;
	}
	void FrameAnimation::removeFrame(int frame)
	{
		if(m_animations.find(m_activeAnimation) != m_animations.end())
		{
			m_animations[m_activeAnimation].erase(m_animations[m_activeAnimation].begin() + frame);
		}
	}
	void FrameAnimation::clearFrames()
	{
		m_animations[m_activeAnimation].clear();
	}

	void FrameAnimation::animate(sf::Shape &shape)
	{
		// Increment counter with delta time
		m_frameDuration += EngineInfo::getDelta();

		if(m_frameDuration >= m_animations[m_activeAnimation][m_frameIndex].frameDuration)
		{
			// Get remainder
			m_frameDuration = m_frameDuration - m_animations[m_activeAnimation][m_frameIndex].frameDuration;

			// Increment frame index
			++m_frameIndex;

			// Change to new frame
			shape.setTextureRect(m_animations[m_activeAnimation][m_frameIndex].frame);
		}
	}
};