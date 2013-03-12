#include "FrameAnimation.h"
#include <stdexcept>
#include "EngineInfo.h"

namespace cc
{
	FrameAnimation::FrameAnimation()
	 :
	 m_frameIndex(0),
	 m_activeAnimation("")
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
	void FrameAnimation::initAnimation(sf::Shape &shape, const std::string &name)
	{
		if(m_animations.find(name) != m_animations.end() && m_animations[name].size() > 0)
			shape.setTextureRect(m_animations[name][0].frame);
	}
	void FrameAnimation::initAnimation(sf::Sprite &sprite, const std::string &name)
	{
		if(m_animations.find(name) != m_animations.end() && m_animations[name].size() > 0)
			sprite.setTextureRect(m_animations[name][0].frame);
	}

	FrameAnimation &FrameAnimation::pushFrame(const sf::IntRect &frame, double frameDuration)
	{
		if(m_animations.find(m_activeAnimation) != m_animations.end())
		{
			Frame data;
			data.frame = frame;
			data.frameDuration = 0;
			data.frameMaxDuration = frameDuration;
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
			m_animations[name].erase(m_animations[name].begin() + frame);
	}
	void FrameAnimation::clearFrames(const std::string &name)
	{
		m_animations[name].clear();
	}

	void FrameAnimation::animate(sf::Shape &shape, const std::string &name)
	{
		// Shortened access to the frame
		Frame &frame = m_animations[m_activeAnimation][m_frameIndex];

		// Increment counter with delta time
		frame.frameDuration += EngineInfo::getDelta();

		// Check if the animation has changed, if so,
		// set first frame of new animation at once
		if(name != m_activeAnimation)
			frame.frameDuration = frame.frameMaxDuration;

		// Set this animation to the active one
		m_activeAnimation = name;

		if(frame.frameDuration >= frame.frameMaxDuration)
		{
			// Get remainder
			frame.frameDuration = frame.frameMaxDuration - frame.frameDuration;

			// Increment frame index
			++m_frameIndex;

			// Make sure index is within bounds
			if(m_frameIndex >= m_animations[m_activeAnimation].size())
				m_frameIndex = 0;

			// Change to new frame, don't use "frame" shortener as index has changed
			shape.setTextureRect(m_animations[m_activeAnimation][m_frameIndex].frame);
		}
	}
	void FrameAnimation::animate(sf::Sprite &sprite, const std::string &name)
	{
		// Check if the animation has changed, if so,
		// set first frame of new animation at once
		if(name != m_activeAnimation)
		{
			// Switch active animation
			m_activeAnimation = name;

			if(m_frameIndex >= m_animations[m_activeAnimation].size())
				m_frameIndex = 0;
			
			sprite.setTextureRect(getFrame().frame);
			//getFrame().frameDuration = getFrame().frameMaxDuration;
		}


		if(getFrame().frameDuration >= getFrame().frameMaxDuration)
		{
			// Get remainder
			getFrame().frameDuration = getFrame().frameMaxDuration - getFrame().frameDuration;

			// Increment frame index
			++m_frameIndex;

			// Make sure index is within bounds
			if(m_frameIndex >= m_animations[m_activeAnimation].size())
				m_frameIndex = 0;

			// Change to new frame, don't use "frame" shortener as index has changed
			sprite.setTextureRect(getFrame().frame);
		}

		// Increment counter with delta time
		else
			m_animations[m_activeAnimation][m_frameIndex].frameDuration += EngineInfo::getDelta();
	}



	FrameAnimation::Frame &FrameAnimation::getFrame()
	{
		return m_animations[m_activeAnimation][m_frameIndex];
	}
};