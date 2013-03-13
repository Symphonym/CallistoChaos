#include "FrameAnimation.h"
#include <stdexcept>
#include "EngineInfo.h"
#include "SoundManager.h"

namespace cc
{
	FrameAnimation::FrameAnimation()
	 :
	 m_frameIndex(0),
	 m_activeAnimation(""),
	 m_frameDuration(0),
	 m_requestedAnimation("")
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

	FrameAnimation &FrameAnimation::pushFrame(const sf::IntRect &frame, double frameDuration, const std::string &soundFilePath)
	{
		if(m_animations.find(m_activeAnimation) != m_animations.end())
		{
			Frame data;
			data.frame = frame;
			data.frameMaxDuration = frameDuration;
			data.soundPath = soundFilePath;
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
		// Make sure the animation exists
		if(m_animations.find(name) == m_animations.end())
			return;

		// Check if the animation has changed, if so,
		// set first frame of new animation at once
		if(name != m_activeAnimation)
		{
			// Switch active animation
			m_activeAnimation = name;

			if(m_frameIndex >= m_animations[m_activeAnimation].size())
				m_frameIndex = 0;
			
			m_frameDuration = 0;
			shape.setTextureRect(getFrame().frame);
		}

		// If no new animation, simply continue updating the animation
		else if(progress())
			shape.setTextureRect(getFrame().frame);
	}
	void FrameAnimation::animate(sf::Sprite &sprite, const std::string &name)
	{
		// Make sure the animation exists
		if(m_animations.find(name) == m_animations.end())
			return;

		// Check if the animation has changed, if so,
		// set first frame of new animation at once
		if(name != m_activeAnimation)
		{
			// Switch active animation
			m_activeAnimation = name;

			if(m_frameIndex >= m_animations[m_activeAnimation].size())
				m_frameIndex = 0;
			
			m_frameDuration = 0;
			sprite.setTextureRect(getFrame().frame);
		}

		// If no new animation, simply continue updating the animation
		else if(progress())
			sprite.setTextureRect(getFrame().frame);
	}

	void FrameAnimation::request(const std::string &name)
	{
		// Request the specifed animation for the upcoming
		// "commit" call
		m_requestedAnimation = name;
	}
	void FrameAnimation::commit(sf::Shape &shape)
	{
		// Make sure the animation exists and has been requested
		if(m_animations.find(m_requestedAnimation) == m_animations.end() || m_requestedAnimation.empty())
			return;

		// Animate sprite with requested animation
		animate(shape, m_requestedAnimation);

		// Reset requested animation
		m_requestedAnimation = "";
	}
	void FrameAnimation::commit(sf::Sprite &sprite)
	{
		// Make sure the animation exists and has been requested
		if(m_animations.find(m_requestedAnimation) == m_animations.end() || m_requestedAnimation.empty())
			return;

		// Animate sprite with requested animation
		animate(sprite, m_requestedAnimation);

		// Reset requested animation
		m_requestedAnimation = "";
	}


	bool FrameAnimation::progress()
	{
		if(m_frameDuration >= getFrame().frameMaxDuration)
		{
			// Get remainder
			m_frameDuration = getFrame().frameMaxDuration - m_frameDuration;
			++m_frameIndex;

			// Make sure index is within bounds
			if(m_frameIndex >= m_animations[m_activeAnimation].size())
				m_frameIndex = 0;

			// Play frame sound, if any
			if(!getFrame().soundPath.empty())
				SoundManager::playSound(getFrame().soundPath);

			return true;
		}
		else
		{
			m_frameDuration += EngineInfo::getDelta();
			return false;
		}
	}
	FrameAnimation::Frame &FrameAnimation::getFrame()
	{
		return m_animations[m_activeAnimation][m_frameIndex];
	}
};