//
//  Text.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/6/13.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "Text.h"

Text::Text(std::string text, float x, float y, float width, float height, float red, float green, float blue, float alpha) : PhysicalEntity(x, y, width, height),
m_strText(text),
m_color(red, green, blue, alpha)
{
	m_strText = text;
}

std::string Text::getText()
{
	return m_strText;
}

void Text::setText(std::string text)
{
	m_strText = text;
}

Color& Text::getColor()
{
    return m_color;
}

void Text::setColor(float red, float green, float blue, float alpha)
{
	m_color.red = red;
	m_color.green = green;
	m_color.blue = blue;
	m_color.alpha = alpha;
}
