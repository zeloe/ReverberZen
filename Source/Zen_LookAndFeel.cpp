/*
  ==============================================================================

    Zen_LookAndFeel.cpp
    Created: 22 Feb 2022 7:39:26pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#include "Zen_LookAndFeel.h"
namespace juce
{
Zen_GreenKnob::Zen_GreenKnob()
{
}

Zen_GreenKnob::~Zen_GreenKnob()
{
}
void Zen_GreenKnob::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                                   const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
        
    const auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
    const auto text    = slider.findColour (juce::Slider::textBoxTextColourId);
    auto color = juce::Colours::green;
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);
    auto centre = bounds.getCentre();

    const auto tickHeight = 6;
    const auto tickWidth  = 40;

    auto radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);

    g.setColour (text);
    if (radius > tickWidth * 2.0f + 10.0f)
    {
        const auto xLeft  = int (bounds.getX());
        const auto xRight = int (bounds.getRight() - tickWidth);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.5f)),   int (centre.getX() - tickWidth / 2), int (bounds.getY()), tickWidth, tickHeight, juce::Justification::centred, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.375f)), xLeft,  int (bounds.getY()), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.25f)),  xLeft,  int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.125f)), xLeft,  int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.0f)),   xLeft,  int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.625f)), xRight, int (bounds.getY()), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.75f)),  xRight, int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.875f)), xRight, int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (1.0f)),   xRight, int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::right, 1);

        bounds.removeFromTop (tickHeight + 4.0f);
        bounds.reduce (tickWidth, 0.0f);
    }

    radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);
    centre = bounds.getCentre();

    if (radius > 50.0f)
    {
        for (int i = 0; i < 9; ++i)
        {
            const auto angle = juce::jmap (i / 8.0f, rotaryStartAngle, rotaryEndAngle);
            const auto point = centre.getPointOnCircumference (radius - 2.0f, angle);
            g.fillEllipse (point.getX() - 3, point.getY() - 3, 6, 6);
        }
        radius -= 10.0f;
    }

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = juce::jmin (4.0f, radius * 0.5f);
    auto arcRadius  = radius - lineW;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);

    g.setColour (outline);
    g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));

    auto knobRadius = std:: max (radius - 3.0f * lineW, 10.0f);
    {
        juce::Graphics::ScopedSaveState saved (g);
        if (slider.isEnabled())
        {
            juce::ColourGradient fillGradient (outline.brighter(), centre.getX() + lineW * 2.0f, centre.getY() - lineW * 4.0f, outline, centre.getX() + knobRadius, centre.getY() + knobRadius, true);
            g.setGradientFill (fillGradient);
        }
        g.setColour (color);
        g.fillEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);
    }

    knobRadius = std:: max (knobRadius - 4.0f, 10.0f);
    g.setColour (outline.brighter());
    g.drawEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f, 2.0f);

    if (slider.isEnabled() && arcRadius > 10.0f)
    {
        juce::Path valueArc;
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);
        g.setColour (juce::Colours::white);
        
        g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));
    }
    g.setColour (juce::Colours::black);
    juce::Path p;
    p.startNewSubPath (centre.getPointOnCircumference (knobRadius - lineW, toAngle));
    p.lineTo (centre.getPointOnCircumference ((knobRadius - lineW) * 0.6f, toAngle));
    g.strokePath (p, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}
Zen_RedKnob::Zen_RedKnob()
{
}

Zen_RedKnob::~Zen_RedKnob()
{
}
void Zen_RedKnob::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                                   const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
        
    const auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
    const auto text    = slider.findColour (juce::Slider::textBoxTextColourId);
    auto color = juce::Colours::red;
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);
    auto centre = bounds.getCentre();

    const auto tickHeight = 6;
    const auto tickWidth  = 40;

    auto radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);

    g.setColour (text);
    if (radius > tickWidth * 2.0f + 10.0f)
    {
        const auto xLeft  = int (bounds.getX());
        const auto xRight = int (bounds.getRight() - tickWidth);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.5f)),   int (centre.getX() - tickWidth / 2), int (bounds.getY()), tickWidth, tickHeight, juce::Justification::centred, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.375f)), xLeft,  int (bounds.getY()), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.25f)),  xLeft,  int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.125f)), xLeft,  int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.0f)),   xLeft,  int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.625f)), xRight, int (bounds.getY()), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.75f)),  xRight, int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.875f)), xRight, int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (1.0f)),   xRight, int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::right, 1);

        bounds.removeFromTop (tickHeight + 4.0f);
        bounds.reduce (tickWidth, 0.0f);
    }

    radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);
    centre = bounds.getCentre();

    if (radius > 50.0f)
    {
        for (int i = 0; i < 9; ++i)
        {
            const auto angle = juce::jmap (i / 8.0f, rotaryStartAngle, rotaryEndAngle);
            const auto point = centre.getPointOnCircumference (radius - 2.0f, angle);
            g.fillEllipse (point.getX() - 3, point.getY() - 3, 6, 6);
        }
        radius -= 10.0f;
    }

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = juce::jmin (4.0f, radius * 0.5f);
    auto arcRadius  = radius - lineW;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);

    g.setColour (outline);
    g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));

    auto knobRadius = std:: max (radius - 3.0f * lineW, 10.0f);
    {
        juce::Graphics::ScopedSaveState saved (g);
        if (slider.isEnabled())
        {
            juce::ColourGradient fillGradient (outline.brighter(), centre.getX() + lineW * 2.0f, centre.getY() - lineW * 4.0f, outline, centre.getX() + knobRadius, centre.getY() + knobRadius, true);
            g.setGradientFill (fillGradient);
        }
        g.setColour (color);
        g.fillEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);
    }

    knobRadius = std:: max (knobRadius - 4.0f, 10.0f);
    g.setColour (outline.brighter());
    g.drawEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f, 2.0f);

    if (slider.isEnabled() && arcRadius > 10.0f)
    {
        juce::Path valueArc;
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);
        g.setColour (juce::Colours::white);
        
        g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));
    }
    g.setColour (juce::Colours::black);
    juce::Path p;
    p.startNewSubPath (centre.getPointOnCircumference (knobRadius - lineW, toAngle));
    p.lineTo (centre.getPointOnCircumference ((knobRadius - lineW) * 0.6f, toAngle));
    g.strokePath (p, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}
Zen_BlueKnob::Zen_BlueKnob()
{
}

Zen_BlueKnob::~Zen_BlueKnob()
{
}
void Zen_BlueKnob::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                                   const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
        
    const auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
    const auto text    = slider.findColour (juce::Slider::textBoxTextColourId);
    auto color = juce::Colours::blue;
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);
    auto centre = bounds.getCentre();

    const auto tickHeight = 6;
    const auto tickWidth  = 40;

    auto radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);

    g.setColour (text);
    if (radius > tickWidth * 2.0f + 10.0f)
    {
        const auto xLeft  = int (bounds.getX());
        const auto xRight = int (bounds.getRight() - tickWidth);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.5f)),   int (centre.getX() - tickWidth / 2), int (bounds.getY()), tickWidth, tickHeight, juce::Justification::centred, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.375f)), xLeft,  int (bounds.getY()), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.25f)),  xLeft,  int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.125f)), xLeft,  int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.0f)),   xLeft,  int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.625f)), xRight, int (bounds.getY()), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.75f)),  xRight, int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.875f)), xRight, int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (1.0f)),   xRight, int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::right, 1);

        bounds.removeFromTop (tickHeight + 4.0f);
        bounds.reduce (tickWidth, 0.0f);
    }

    radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);
    centre = bounds.getCentre();

    if (radius > 50.0f)
    {
        for (int i = 0; i < 9; ++i)
        {
            const auto angle = juce::jmap (i / 8.0f, rotaryStartAngle, rotaryEndAngle);
            const auto point = centre.getPointOnCircumference (radius - 2.0f, angle);
            g.fillEllipse (point.getX() - 3, point.getY() - 3, 6, 6);
        }
        radius -= 10.0f;
    }

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = juce::jmin (4.0f, radius * 0.5f);
    auto arcRadius  = radius - lineW;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);

    g.setColour (outline);
    g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));

    auto knobRadius = std:: max (radius - 3.0f * lineW, 10.0f);
    {
        juce::Graphics::ScopedSaveState saved (g);
        if (slider.isEnabled())
        {
            juce::ColourGradient fillGradient (outline.brighter(), centre.getX() + lineW * 2.0f, centre.getY() - lineW * 4.0f, outline, centre.getX() + knobRadius, centre.getY() + knobRadius, true);
            g.setGradientFill (fillGradient);
        }
        g.setColour (color);
        g.fillEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);
    }

    knobRadius = std:: max (knobRadius - 4.0f, 10.0f);
    g.setColour (outline.brighter());
    g.drawEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f, 2.0f);

    if (slider.isEnabled() && arcRadius > 10.0f)
    {
        juce::Path valueArc;
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);
        g.setColour (juce::Colours::white);
      
        g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));
    }
    g.setColour (juce::Colours::black);
    juce::Path p;
    p.startNewSubPath (centre.getPointOnCircumference (knobRadius - lineW, toAngle));
    p.lineTo (centre.getPointOnCircumference ((knobRadius - lineW) * 0.6f, toAngle));
    g.strokePath (p, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}

Zen_WhiteKnob::Zen_WhiteKnob()
{
}

Zen_WhiteKnob::~Zen_WhiteKnob()
{
}
void Zen_WhiteKnob::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                                   const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    const auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
    const auto text    = slider.findColour (juce::Slider::textBoxTextColourId);
    auto color = juce::Colours::white;
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);
    auto centre = bounds.getCentre();

    const auto tickHeight = 6;
    const auto tickWidth  = 40;

    auto radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);

    g.setColour (text);
    if (radius > tickWidth * 2.0f + 10.0f)
    {
        const auto xLeft  = int (bounds.getX());
        const auto xRight = int (bounds.getRight() - tickWidth);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.5f)),   int (centre.getX() - tickWidth / 2), int (bounds.getY()), tickWidth, tickHeight, juce::Justification::centred, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.375f)), xLeft,  int (bounds.getY()), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.25f)),  xLeft,  int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.125f)), xLeft,  int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.0f)),   xLeft,  int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.625f)), xRight, int (bounds.getY()), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.75f)),  xRight, int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.875f)), xRight, int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (1.0f)),   xRight, int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::right, 1);

        bounds.removeFromTop (tickHeight + 4.0f);
        bounds.reduce (tickWidth, 0.0f);
    }

    radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);
    centre = bounds.getCentre();

    if (radius > 50.0f)
    {
        for (int i = 0; i < 9; ++i)
        {
            const auto angle = juce::jmap (i / 8.0f, rotaryStartAngle, rotaryEndAngle);
            const auto point = centre.getPointOnCircumference (radius - 2.0f, angle);
            g.fillEllipse (point.getX() - 3, point.getY() - 3, 6, 6);
        }
        radius -= 10.0f;
    }

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = juce::jmin (4.0f, radius * 0.5f);
    auto arcRadius  = radius - lineW;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);

    g.setColour (outline);
    g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));

    auto knobRadius = std:: max (radius - 3.0f * lineW, 10.0f);
    {
        juce::Graphics::ScopedSaveState saved (g);
        if (slider.isEnabled())
        {
            juce::ColourGradient fillGradient (outline.brighter(), centre.getX() + lineW * 2.0f, centre.getY() - lineW * 4.0f, outline, centre.getX() + knobRadius, centre.getY() + knobRadius, true);
            g.setGradientFill (fillGradient);
        }
        g.setColour (color);
        g.fillEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);
    }

    knobRadius = std:: max (knobRadius - 4.0f, 10.0f);
    g.setColour (outline.brighter());
    g.drawEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f, 2.0f);

    if (slider.isEnabled() && arcRadius > 10.0f)
    {
        juce::Path valueArc;
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);

        g.setColour (juce::Colours::white);
        g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));
    }
    
    g.setColour (juce::Colours::black);
    juce::Path p;
    p.startNewSubPath (centre.getPointOnCircumference (knobRadius - lineW, toAngle));
    p.lineTo (centre.getPointOnCircumference ((knobRadius - lineW) * 0.6f, toAngle));
    g.strokePath (p, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}


Zen_BrownKnob::Zen_BrownKnob()
{
}

Zen_BrownKnob::~Zen_BrownKnob()
{
}
void Zen_BrownKnob::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                                   const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
        
    const auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
    const auto text    = slider.findColour (juce::Slider::textBoxTextColourId);
    auto color = juce::Colours::brown;
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);
    auto centre = bounds.getCentre();

    const auto tickHeight = 6;
    const auto tickWidth  = 40;

    auto radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);

    g.setColour (text);
    if (radius > tickWidth * 2.0f + 10.0f)
    {
        const auto xLeft  = int (bounds.getX());
        const auto xRight = int (bounds.getRight() - tickWidth);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.5f)),   int (centre.getX() - tickWidth / 2), int (bounds.getY()), tickWidth, tickHeight, juce::Justification::centred, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.375f)), xLeft,  int (bounds.getY()), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.25f)),  xLeft,  int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.125f)), xLeft,  int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.0f)),   xLeft,  int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::left, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.625f)), xRight, int (bounds.getY()), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.75f)),  xRight, int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.875f)), xRight, int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
        g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (1.0f)),   xRight, int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::right, 1);

        bounds.removeFromTop (tickHeight + 4.0f);
        bounds.reduce (tickWidth, 0.0f);
    }

    radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);
    centre = bounds.getCentre();

    if (radius > 50.0f)
    {
        for (int i = 0; i < 9; ++i)
        {
            const auto angle = juce::jmap (i / 8.0f, rotaryStartAngle, rotaryEndAngle);
            const auto point = centre.getPointOnCircumference (radius - 2.0f, angle);
            g.fillEllipse (point.getX() - 3, point.getY() - 3, 6, 6);
        }
        radius -= 10.0f;
    }

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = juce::jmin (4.0f, radius * 0.5f);
    auto arcRadius  = radius - lineW;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);

    g.setColour (outline);
    g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));

    auto knobRadius = std:: max (radius - 3.0f * lineW, 10.0f);
    {
        juce::Graphics::ScopedSaveState saved (g);
        if (slider.isEnabled())
        {
            juce::ColourGradient fillGradient (outline.brighter(), centre.getX() + lineW * 2.0f, centre.getY() - lineW * 4.0f, outline, centre.getX() + knobRadius, centre.getY() + knobRadius, true);
            g.setGradientFill (fillGradient);
        }
        g.setColour (color);
        g.fillEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);
    }

    knobRadius = std:: max (knobRadius - 4.0f, 10.0f);
    g.setColour (outline.brighter());
    g.drawEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f, 2.0f);

    if (slider.isEnabled() && arcRadius > 10.0f)
    {
        juce::Path valueArc;
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);

        g.setColour (juce::Colours::white);
        g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));
    }
   
    g.setColour (juce::Colours::black);
    juce::Path p;
    p.startNewSubPath (centre.getPointOnCircumference (knobRadius - lineW, toAngle));
    p.lineTo (centre.getPointOnCircumference ((knobRadius - lineW) * 0.6f, toAngle));
    g.strokePath (p, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}


Zen_LightBlueKnob::Zen_LightBlueKnob()
{
}

Zen_LightBlueKnob::~Zen_LightBlueKnob()
{
}
void Zen_LightBlueKnob::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                                   const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    
        const auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
        const auto text    = slider.findColour (juce::Slider::textBoxTextColourId);
        auto color = juce::Colours::lightblue;
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);
        auto centre = bounds.getCentre();

        const auto tickHeight = 6;
        const auto tickWidth  = 40;

        auto radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);

        g.setColour (text);
        if (radius > tickWidth * 2.0f + 10.0f)
        {
            const auto xLeft  = int (bounds.getX());
            const auto xRight = int (bounds.getRight() - tickWidth);
            g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.5f)),   int (centre.getX() - tickWidth / 2), int (bounds.getY()), tickWidth, tickHeight, juce::Justification::centred, 1);
            g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.375f)), xLeft,  int (bounds.getY()), tickWidth, tickHeight, juce::Justification::left, 1);
            g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.25f)),  xLeft,  int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
            g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.125f)), xLeft,  int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::left, 1);
            g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.0f)),   xLeft,  int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::left, 1);
            g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.625f)), xRight, int (bounds.getY()), tickWidth, tickHeight, juce::Justification::right, 1);
            g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.75f)),  xRight, int (juce::jmap (0.33f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
            g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (0.875f)), xRight, int (juce::jmap (0.66f,  float (bounds.getY()), bounds.getBottom() - tickHeight)), tickWidth, tickHeight, juce::Justification::right, 1);
            g.drawFittedText (slider.getTextFromValue (slider.proportionOfLengthToValue (1.0f)),   xRight, int (bounds.getBottom() - tickHeight), tickWidth, tickHeight, juce::Justification::right, 1);

            bounds.removeFromTop (tickHeight + 4.0f);
            bounds.reduce (tickWidth, 0.0f);
        }

        radius = juce::jmin (bounds.getWidth() / 2.0f, bounds.getHeight() / 2.0f);
        centre = bounds.getCentre();

        if (radius > 50.0f)
        {
            for (int i = 0; i < 9; ++i)
            {
                const auto angle = juce::jmap (i / 8.0f, rotaryStartAngle, rotaryEndAngle);
                const auto point = centre.getPointOnCircumference (radius - 2.0f, angle);
                g.fillEllipse (point.getX() - 3, point.getY() - 3, 6, 6);
            }
            radius -= 10.0f;
        }

        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin (4.0f, radius * 0.5f);
        auto arcRadius  = radius - lineW;

        juce::Path backgroundArc;
        backgroundArc.addCentredArc (bounds.getCentreX(),
                                     bounds.getCentreY(),
                                     arcRadius,
                                     arcRadius,
                                     0.0f,
                                     rotaryStartAngle,
                                     rotaryEndAngle,
                                     true);

        g.setColour (outline);
        g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));

        auto knobRadius = std:: max (radius - 3.0f * lineW, 10.0f);
        {
            juce::Graphics::ScopedSaveState saved (g);
            if (slider.isEnabled())
            {
                juce::ColourGradient fillGradient (outline.brighter(), centre.getX() + lineW * 2.0f, centre.getY() - lineW * 4.0f, outline, centre.getX() + knobRadius, centre.getY() + knobRadius, true);
                g.setGradientFill (fillGradient);
            }
            g.setColour (color);
            g.fillEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);
        }

        knobRadius = std:: max (knobRadius - 4.0f, 10.0f);
        g.setColour (outline.brighter());
        g.drawEllipse (centre.getX() - knobRadius, centre.getY() - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f, 2.0f);

        if (slider.isEnabled() && arcRadius > 10.0f)
        {
            juce::Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(),
                                    bounds.getCentreY(),
                                    arcRadius,
                                    arcRadius,
                                    0.0f,
                                    rotaryStartAngle,
                                    toAngle,
                                    true);

            g.setColour (juce::Colours::white);
            g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::butt));
        }
        g.setColour (juce::Colours::black);
        juce::Path p;
        p.startNewSubPath (centre.getPointOnCircumference (knobRadius - lineW, toAngle));
        p.lineTo (centre.getPointOnCircumference ((knobRadius - lineW) * 0.6f, toAngle));
        g.strokePath (p, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }
     /*
    auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
    auto centreX = (float) x + (float) width  * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
// fill
    g.setColour (juce::Colours::lightblue);
    g.fillEllipse (rx, ry, rw, rw);
             
// outline
    g.setColour (juce::Colours::black);
    g.drawEllipse (rx, ry, rw, rw, 2.0f);
            
// outline2 lines
   // g.setColour (juce::Colours::blue);
   // g.drawRect(g.getClipBounds());
//  body
    juce::Path p;
    auto pointerLength = radius - 50.f;
    auto pointerThickness = 3.0f;
    juce::Point<float> point1;
    juce::Point<float> point2;
    juce::Point<float> point3;
    point1.addXY(pointerLength, pointerLength);
    point2.addXY(radius, pointerLength);
    point3.addXY(-radius, pointerLength);
    p.addTriangle(pointerLength + 1.f ,centreY, pointerLength, centreX,pointerThickness,pointerLength);
   // p.addRectangle (-pointerThickness , -radius, pointerThickness, pointerLength);
    p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
            
    // pointer
    g.setColour (juce::Colours::black);
    g.fillPath (p);
     
}
      */

}
