#include <SFML/Graphics.hpp>

static const unsigned int MAX_ITER;
static const float BASE_WIDTH;
static const float BASE_HEIGHT;
static const float BASE_ZOOM;

class ComplexPlane : public sf::Drawable
{
    public:
        

        enum class State
        {
            CALCULATING,
            DISPLAYING
        };

        ComplexPlane(int pixelWidth, int pixelHeight);

        void setState(State state);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void zoomIn();
        void zoomOut();
        void setCenter(sf::Vector2i mousePixel);
        void setMouseLocation(sf::Vector2i mousePixel);
        void loadText(sf::Text& text);
        void updateRender();
    private:
        sf::VertexArray m_vArray;
        State m_state;
        sf::Vector2f m_mouseLocation;
        sf::Vector2i m_pixelSize;
        sf::Vector2f m_planeCenter;
        sf::Vector2f m_planeSize;
        int m_zoomCount;
        float m_aspectRatio;

        size_t counterIterations(sf::Vector2f coord);
        void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b);
        sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel);

};      