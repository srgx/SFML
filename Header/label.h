#ifndef LABEL__H__
#define LABEL__H__

#include "component.h"
#include "resource_identifiers.h"
#include "resource_holder.h"

#include <SFML/Graphics/Text.hpp>


namespace GUI{

class Label : public Component{
public:
  typedef std::shared_ptr<Label> Ptr;
public:
    Label(const std::string& text, const FontHolder& fonts);
    virtual bool isSelectable() const;
    void setText(const std::string& text);
    virtual void handleEvent(const sf::Event& event);
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    sf::Text mText;
};

}

#endif
