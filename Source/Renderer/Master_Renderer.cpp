#include "Master_Renderer.h"

#include "../D_Settings.h"
#include "../Util/Display.h"

void Master_Renderer::prepare()
{
    m_framebuffer.clear();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(Settings::SKY_RED, Settings::SKY_GREEN, Settings::SKY_BLUE, 1.0);
}


void Master_Renderer::processChunk(const Chunk& chunk)
{
    m_chunkRenderer.addChunk    (chunk);
    m_waterRenderer.addChunk    (chunk);
    m_floraRenderer.addChunk      (chunk);
}

void Master_Renderer::processSfDrawable(const sf::Drawable& sfDrawable)
{
    m_sfmlRenderer.addSfDrawable(sfDrawable);
}

void Master_Renderer::render (const Camera& camera)
{
    //Draw the scene (to the FBO)
    drawScene(camera);

    //Draw to the default context
    m_mainShader.useProgram();
    m_framebuffer.draw();

    //Finally, draw SFML stuff.
    m_sfmlRenderer .render  ();


    Display::update();
}

void Master_Renderer::drawScene(const Camera& camera)
{
    glEnable(GL_DEPTH_TEST);
    m_framebuffer.bind();

    m_chunkRenderer.render  (camera);
    m_waterRenderer.render  (camera);
    m_floraRenderer.render  (camera);

    m_framebuffer.unbind();
    glDisable(GL_DEPTH_TEST);
}
