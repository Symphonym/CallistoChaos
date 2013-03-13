#ifndef CC_FRAMEANIMATION_H
#define CC_FRAMEANIMATION_H

#include <SFML/Graphics.hpp>

namespace cc
{
	// Allows to create FrameAnimation using an sf::Shape to
	// change it's subrect in order to render different parts
	// of the texture it's using. Also it has support for
	// multiple animations in the same object, as well as
	// multiple animation methods (2 methods).
	class FrameAnimation
	{
	private:

		struct Frame
		{
			// Frame and lifetime of the frame
			sf::IntRect frame;
			double frameMaxDuration;
		};

		// All the Animations
		std::map<std::string, std::vector<Frame>> m_animations;

		// Index of the current frame
		std::size_t m_frameIndex;
		// String ID of the active animation
		std::string m_activeAnimation;
		// How long the frame has been active, in seconds
		double m_frameDuration;

		// Animation method 2 variables
		std::string m_requestedAnimation;

		// Returns the active frame, m_animations[m_activeAnimation][m_frameIndex]
		Frame &getFrame();

		// Updates the animation, returns true if frame was changed
		bool progress();

	public:

		explicit FrameAnimation();

		// Creates a new animation, making it active to
		// allow for pushFrame calls
		void createAnimation(const std::string &name);
		// Deletes the specified animation
		void deleteAnimation(const std::string &name);
		// Initializes the shape to the first frame of the animation
		void initAnimation(sf::Shape &shape, const std::string &name);
		// Initializes the sprite to the first frame of the animation
		void initAnimation(sf::Sprite &sprite, const std::string &name);

		// Pushes a new frame onto the active animation, returns this to allow chaining calls
		FrameAnimation &pushFrame(const sf::IntRect &frame, double frameDuration);
		// Removes the specified frame from the specified animation
		void removeFrame(const std::string& name, std::size_t frameIndex);
		// Removes all frames from the specified animation
		void clearFrames(const std::string &name);

		// Provides two ways of animating the sprite, both of
		// the methods cannot be used on the same object.

		// Animation method 1
		// Animates the sprite/shape directly by calling
		// "animate". Cannot run multiple "animate" calls
		// in the same tick, use a string to set, and run
		// "animate" once at the end.
		void animate(sf::Shape &shape, const std::string &name);
		void animate(sf::Sprite &sprite, const std::string &name);

		// Animation method 2
		// Animates the sprite/shape indirectly by first
		// "request"ing the animation to be updated. This
		// function can then be called multiple times in 
		// the same tick, the last call will be the animation
		// that's updated when "commit" is called.

		// Request an animation to be updated
		// when "commit" is called
		void request(const std::string &name);

		// Updates the the animation with the last animation
		// which was specified through "request"
		void commit(sf::Shape &shape);
		void commit(sf::Sprite &sprite);
	};
};

#endif