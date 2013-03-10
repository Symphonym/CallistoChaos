#ifndef CC_RENDERER_H
#define CC_RENDERER_H

#include <map>
#include <SFML/Graphics.hpp>

namespace cc
{
	class Renderer
	{
	private:
		
		// Pointer to Rendertarget onto which rendering occurs
		sf::RenderTarget *m_rendertarget;

	public:

		explicit Renderer();

		// Factory method
		// Creates and returns a centered View, generated from the
		// Rendertarget's default view. Using the specified zoom factor.
		sf::View createView(double zoomFactor = 1.0);

		// Set rendertarget to render onto
		void setRenderTarget(sf::RenderTarget &target);

		// Set active View used for rendering
		void setView(const sf::View& view);

		const sf::View &getView() const;
		sf::Vector2f getViewSize();
		const sf::RenderTarget &getTarget() const;

		// Render drawable to the specified Rendertarget, using the specified View
		void render(const sf::Drawable &drawable, const sf::RenderStates &states = sf::RenderStates::Default);
	};
};

#endif