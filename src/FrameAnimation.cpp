#include "FrameAnimation.h"
#include <stdexcept>
#include "EngineInfo.h"
#include <iostream>

namespace cc
{
	FrameAnimation::FrameAnimation()
	 :
	 m_frameDuration(0),
	 m_frameIndex(0),
	 m_activeAnimation(""),
	 m_hasChangedAnimation(true)
	{

	}

	void FrameAnimation::createAnimation(const std::string &name)
	{
		std::vector<Frame> frames;
		m_animations[name] = frames;

		// Set this to active animation to allow pushFrame calls
		m_activeAnimation = name;
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
	void FrameAnimation::removeFrame(const std::string& name, std::size_t frame)
	{
		if(m_animations.find(name) != m_animations.end())
		{
			m_animations[name].erase(m_animations[name].begin() + frame);
		}
	}
	void FrameAnimation::clearFrames(const std::string &name)
	{
		m_animations[name].clear();
	}

	void FrameAnimation::animate(sf::Shape &shape, const std::string &name)
	{
		// Increment counter with delta time
		m_frameDuration += EngineInfo::getDelta();

		// Check if the animation has changed, if so,
		// set first frame of new animation at once
		if(name != m_activeAnimation)
		{
			m_frameDuration = m_animations[m_activeAnimation][m_frameIndex].frameDuration;
			std::cout << "First animation: " << name << std::endl;
		}

		// Set this animation to the active one
		m_activeAnimation = name;

		if(m_frameDuration >= m_animations[m_activeAnimation][m_frameIndex].frameDuration)
		{
			std::cout << "Switched frame: " << name << std::endl;
			// Get remainder
			m_frameDuration = m_frameDuration - m_animations[m_activeAnimation][m_frameIndex].frameDuration;

			// Increment frame index
			++m_frameIndex;

			// Make sure index is within bounds
			if(m_frameIndex >= m_animations[m_activeAnimation].size())
				m_frameIndex = 0;

			// Change to new frame
			shape.setTextureRect(m_animations[m_activeAnimation][m_frameIndex].frame);
		}
	}
	void FrameAnimation::animate(sf::Sprite &sprite, const std::string &name)
	{
		// Increment counter with delta time
		m_frameDuration += EngineInfo::getDelta();

		// Check if the animation has changed, if so,
		// set first frame of new animation at once
		if(name != m_activeAnimation)
			m_frameDuration = m_animations[m_activeAnimation][m_frameIndex].frameDuration;

		// Set this animation to the active one
		m_activeAnimation = name;

		if(m_frameDuration >= m_animations[m_activeAnimation][m_frameIndex].frameDuration)
		{
			// Get remainder
			m_frameDuration = m_frameDuration - m_animations[m_activeAnimation][m_frameIndex].frameDuration;

			// Increment frame index
			++m_frameIndex;

			// Make sure index is within bounds
			if(m_frameIndex >= m_animations[m_activeAnimation].size())
				m_frameIndex = 0;

			// Change to new frame
			sprite.setTextureRect(m_animations[m_activeAnimation][m_frameIndex].frame);
		}
	}
};