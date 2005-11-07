#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Color
//
//////////////////////////////////////////////////////////////////////////////

float HueComponent(float hue)
{
    hue = abs(mod(hue + 3, 6) - 3);

    if (hue > 2) {
        return 0;
    } else if (hue > 1) {
        return (2 - hue);
    } else {
        return 1;
    }
}

void Color::SetHSBA(float hue, float saturation, float brightness, float alpha)
{
    hue = hue * 6;
    float white = (1 - saturation);
    m_r = brightness * (white + saturation * HueComponent(hue       ));
    m_g = brightness * (white + saturation * HueComponent(hue - 2.0f));
    m_b = brightness * (white + saturation * HueComponent(hue + 2.0f));
    m_a = alpha;
}

void Color::GetHSB(float& h, float& s, float& b)
{
    float c0 = m_r;
    float cm = m_g;
    float c1 = m_b;

    //
    // find the largest component
    //

    if (c0 > cm) {
        swap(c0, cm);
    }

    if (cm > c1) {
        swap(cm, c1);
    }

    if (c0 > cm) {
        swap(c0, cm);
    }

    //
    // solve for h, s, b
    //

    b = c1;
    s = (b - c0) / b;
    h = (cm  + b * (s - 1)) / (b * s);

    //
    // shift the hue
    //

    if (c1 == m_b) {
        // blue is the largest component
        
        if (m_g == c0) {
            h = -2 + h;
        } else {
            h = -2 - h;
        }
    } else if (c1 == m_g) {
        // green is the largest component

        if (m_r == c0) {
            h = 2 + h;
        } else {
            h = 2 - h;
        }
    } else {
        // red is the larget component

        if (m_b == c0) {
            h =  h;
        } else {
            h = -h;
        }
    }

    h = h / 6.0f;
}

const Color   Color::s_colorWhite(1, 1, 1);
const Color     Color::s_colorRed(1, 0, 0);
const Color   Color::s_colorGreen(0, 1, 0);
const Color    Color::s_colorBlue(0, 0, 1);
const Color  Color::s_colorYellow(1, 1, 0);
const Color Color::s_colorMagenta(1, 0, 1);
const Color    Color::s_colorCyan(0, 1, 1);
const Color   Color::s_colorBlack(0, 0, 0);
const Color   Color::s_colorGray(0.5f, 0.5f, 0.5f);
