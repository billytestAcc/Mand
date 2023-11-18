#include "ComplexPlane.h"
#include <sstream>
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixelSize.x = pixelWidth;
    m_pixelSize.y = pixelHeight;

    m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;

    m_planeCenter = { 0, 0 };

    m_planeSize = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };

    m_zoomCount = 0;
    m_state = State::CALCULATING;

    m_vArray.setPrimitiveType(sf::Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
    if (m_state == State::CALCULATING)
    {
        for (int i = 0; i < m_pixelSize.y; ++i)
        {
            for (int j = 0; j < m_pixelSize.x; ++j)
            {
                m_vArray[j + i * m_pixelSize.x].position = { static_cast<float>(j), static_cast<float>(i) };
                sf::Vector2f complexCoord = mapPixelToCoords(sf::Vector2i(j, i));
                int iterations = counterIterations(complexCoord);

                sf::Uint8 r, g, b;
                iterationsToRGB(iterations, r, g, b);

                m_vArray[j + i * m_pixelSize.x].color = { r, g, b };
            }
        }
        m_state = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn()
{
    ++m_zoomCount;

    float newSizeX = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float newSizeY = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

    m_planeSize = { newSizeX, newSizeY };

    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
    --m_zoomCount;

    float newSizeX = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float newSizeY = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

    m_planeSize = { newSizeX, newSizeY };
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(sf::Vector2i mousePixel)
{
    m_planeCenter = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel)
{
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(sf::Text& text)
{
    stringstream ss;
    ss << "Mandelbrot Set\n";
    ss << "Center: (" << m_planeCenter.x << ", " << m_planeCenter.y << ")\n";
    ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
    ss << "Left-click to Zoom in\n";
    ss << "Right-click to Zoom out";

    text.setString(ss.str());
}

size_t ComplexPlane::counterIterations(sf::Vector2f coord)
{
    sf::Vector2f z = coord;
    size_t count = 0;

    while (count < MAX_ITER)
    {
        float x = z.x * z.x * z.y + coord.x;
        float y = 2.0f * z.x * z.y + coord.y;

        z = sf::Vector2f(x, y);

        if (z.x * z.x + z.y * z.y > 4.0f)
            break;
        
        count++;
    }

    return count;

}

void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b)
{
    const size_t MAX_ITER = 64; // Adjust based on your application
    const size_t regionCount = 5;
    const size_t regionSize = MAX_ITER / regionCount;

    // Ensure count is within valid range
    count = std::min(count, MAX_ITER - 1);

    // Ensure regionSize is not zero to prevent division by zero
    if (regionSize == 0) {
        r = g = b = 0; // Handle division by zero or choose an appropriate default value
        return;
    }

    size_t regionIndex = count / regionSize;

    switch (regionIndex)
    {
    case 0: // Purple / blue
        r = static_cast<sf::Uint8>(count % regionSize * (255 / regionSize));
        g = 0;
        b = 255;
        break;
    case 1: // Turquoise
        r = 0;
        g = static_cast<sf::Uint8>(count % regionSize * (255 / regionSize));
        b = 255;
        break;
    case 2: // Green
        r = 0;
        g = 255;
        b = static_cast<sf::Uint8>(255 - count % regionSize * (255 / regionSize));
        break;
    case 3: // Yellow
        r = static_cast<sf::Uint8>(count % regionSize * (255 / regionSize));
        g = 255;
        b = 0;
        break;
    case 4: // Red
        r = 255;
        g = static_cast<sf::Uint8>(255 - count % regionSize * (255 / regionSize));
        b = 0;
        break;
    default:
        r = g = b = 0; // Handle unknown region
    }
}


sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel)
{
    float xMin = m_planeCenter.x - m_planeSize.x / 2.0f;
    float xMax = m_planeCenter.x + m_planeSize.x / 2.0f;
    float yMin = m_planeCenter.y - m_planeSize.y / 2.0f;
    float yMax = m_planeCenter.y + m_planeSize.y / 2.0f;

    float xCoord = ((mousePixel.x - 0) / static_cast<float>(m_pixelSize.x)) * (xMax - xMin) + xMin;
    float yCoord = ((mousePixel.y - 0) / static_cast<float>(m_pixelSize.y)) * (yMin - yMax) + yMax;

    return sf::Vector2f(xCoord, yCoord);
}

void ComplexPlane::setState(State state)
{
    m_state = state;
}