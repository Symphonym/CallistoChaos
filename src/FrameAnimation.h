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

	public:

		explicit FrameAnimation();

		// Creates a new animation, also makes it the active one
		void createAnimation(const std::string &name);
		// Sets the active animation by it's string ID
		void setAnimation(const std::string &name, sf::Shape &shape);
		// Deletes the specified animation
		void deleteAnimation(const std::string &name);

		// Pushes a new frame onto the active animation, returns this to allow chaining calls
		FrameAnimation &pushFrame(const sf::IntRect &frame, double frameDuration);
		// Removes the specified frame from the active animation
		void removeFrame(int frame);
		// Removes all frames from the active animation
		void clearFrames();

		// Animates the shape by looping through the specified Frames,
		// changing the shape's texture subrect.
		void animate(sf::Shape &shape);
	};
};

#endif