#include "Renderer.h"

namespace cc
{
	Renderer::Renderer()
	 :
	 m_rendertarget(nullptr)
	{

	}

	sf::View Renderer::createView(double zoomFactor)
	{
		sf::View view(m_rendertarget->getDefaultView());
		view.zoom(zoomFactor);
		view.setCenter(view.getSize().x/2, view.getSize().y/2);

		return view;
	}

	void Renderer::setRenderTarget(sf::RenderTarget &target)
	{
		m_rendertarget = &target;
	}
	void Renderer::setView(const sf::View& view)
	{
		m_rendertarget->setView(view);
	}


	const sf::View &Renderer::getView() const
	{
		return m_rendertarget->getView();
	}
	sf::Vector2f Renderer::getViewSize()
	{
		return m_rendertarget->getView().getSize();
	}
	const sf::RenderTarget &Renderer::getTarget() const
	{
		return *m_rendertarget;
	}

	void Renderer::render(const sf::Drawable &drawable, const sf::RenderStates &states)
	{
		if(m_rendertarget != nullptr)
			m_rendertarget->draw(drawable, states);
	}
};