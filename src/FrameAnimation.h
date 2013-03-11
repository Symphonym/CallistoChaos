#ifndef CC_FRAMEANIMATION_H
#define CC_FRAMEANIMATION_H

#include <SFML/Graphics.hpp>

namespace cc
{
	// Allows to create FrameAnimation using an sf::Shape to
	// change it's subrect in order to render different parts
	// of the texture it's using. Also it has support for
	// multiple animations in the same object.
	class FrameAnimation
	{
	private:

		struct Frame
		{
			// The texture subrect
			sf::IntRect frame;
			// Duration of the frame in seconds
			double frameDuration;
		};

		// All the Animations
		std::map<std::string, std::vector<Frame>> m_animations;

		// How long the current frame has been active
		double m_frameDuration;
		// Index of the current frame
		std::size_t m_frameIndex;
		// String ID of the active animation
		std::string m_activeAnimation;
		// Boolean for when switching between animations,
		// to make sure when an animation is switched it will
		// immediately switch frame and not wait for the frameDuration
		bool m_hasChangedAnimation;

	public:

		explicit FrameAnimation();

		// Creates a new animation, making it active to
		// allow for pushFrame calls
		void createAnimation(const std::string &name);
		// Deletes the specified animation
		void deleteAnimation(const std::string &name);

		// Pushes a new frame onto the active animation, returns this to allow chaining calls
		FrameAnimation &pushFrame(const sf::IntRect &frame, double frameDuration);
		// Removes the specified frame from the specified animation
		void removeFrame(const std::string& name, std::size_t frameIndex);
		// Removes all frames from the specified animation
		void clearFrames(const std::string &name);

		// Animates the shape by looping through the specified Frames,
		// changing the shape's texture subrect.
		void animate(sf::Shape &shape, const std::string &name);
		// Animates the sprite by looping through the specified Frames,
		// changing the sprite's texture subrect.
		void animate(sf::Sprite &sprite, const std::string &name);
	};
};

#endif