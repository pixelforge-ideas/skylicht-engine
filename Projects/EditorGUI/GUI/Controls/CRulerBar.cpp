/*
!@
MIT License

Copyright (c) 2020 Skylicht Technology CO., LTD

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This file is part of the "Skylicht Engine".
https://github.com/skylicht-lab/skylicht-engine
!#
*/

#include "pch.h"
#include "CRulerBar.h"
#include "GUI/Theme/ThemeConfig.h"

namespace Skylicht
{
	namespace Editor
	{
		namespace GUI
		{
			CRulerBar::CRulerBar(CBase* parent, bool horizontal) :
				CBase(parent),
				m_isHorizontal(horizontal),
				m_beginOffset(0.0f),
				m_position(0.0f),
				m_pixelPerUnit(5.0f),
				m_unitScale(1.0f),
				m_textPerUnit(10),
				m_cursorPosition(0.0f),
				m_drawCursorLine(false)
			{
				if (m_isHorizontal)
				{
					setHeight(40.0f);
					dock(EPosition::Top);
				}
				else
				{
					setWidth(40.0f);
					dock(EPosition::Left);
				}

				enableClip(true);
				enableRenderFillRect(true);
				setFillRectColor(ThemeConfig::RulerBG);
			}

			CRulerBar::~CRulerBar()
			{

			}

			void CRulerBar::render()
			{
				CBase::render();

				CRenderer* renderer = CRenderer::getRenderer();

				float size = 0.0f;
				float size1 = 5.0f;		// 1
				float size2 = 10.0f;	// 2
				float size3 = 15.0f;	// 5
				float size4 = 30.0f;	// 10

				// convert the position to unit
				int unit = (int)(m_position / m_pixelPerUnit);

				// begin unit from m_position (round)
				float offset = m_position - unit * m_pixelPerUnit;

				SGUIColor lineColor = ThemeConfig::RulerLine1;
				wchar_t textValue[64];

				if (m_isHorizontal)
				{
					float x = m_beginOffset - offset;
					float w = width();

					while (x < w)
					{
						size = size1;
						lineColor = ThemeConfig::RulerLine1;

						if (unit % 2 == 0)
						{
							size = size2;
							lineColor = ThemeConfig::RulerLine1;
						}
						if (unit % 5 == 0)
						{
							size = size3;
							lineColor = ThemeConfig::RulerLine2;
						}
						if (unit % 10 == 0)
						{
							size = size4;
							lineColor = ThemeConfig::RulerLine3;
						}

						renderer->drawLineY(x, 0.0f, size, lineColor);

						if (unit % m_textPerUnit == 0)
						{
							SRect r;
							r.X = x + 2.0f;
							r.Y = 15.0f;
							r.Width = 9.0f * m_pixelPerUnit;
							r.Height = 20.0f;
							swprintf(textValue, 64, L"%d", (int)(unit * m_unitScale));
							renderer->renderText(r, SizeNormal, ThemeConfig::DefaultTextColor, std::wstring(textValue));
						}

						x = x + m_pixelPerUnit;
						unit++;
					}

					if (m_drawCursorLine)
					{
						float x = m_beginOffset - m_position + m_cursorPosition;
						renderer->drawLineY(x, 0.0f, size4, ThemeConfig::RulerCursor);
					}
				}
				else
				{
					float y = m_beginOffset - offset;
					float h = height();

					while (y < h)
					{
						size = size1;
						lineColor = ThemeConfig::RulerLine1;

						if (unit % 2 == 0)
						{
							size = size2;
							lineColor = ThemeConfig::RulerLine1;
						}
						if (unit % 5 == 0)
						{
							size = size3;
							lineColor = ThemeConfig::RulerLine2;
						}
						if (unit % 10 == 0)
						{
							size = size4;
							lineColor = ThemeConfig::RulerLine3;
						}

						renderer->drawLineX(0.0f, y, size, lineColor);

						if (unit % m_textPerUnit == 0)
						{
							SRect r;
							r.X = 15.0f;
							r.Y = y;
							r.Width = 20.0f;
							r.Height = 20.0f;
							swprintf(textValue, 64, L"%d", (int)(unit * m_unitScale));
							renderer->renderText(r, SizeNormal, ThemeConfig::DefaultTextColor, std::wstring(textValue));
						}

						y = y + m_pixelPerUnit;
						unit++;
					}

					if (m_drawCursorLine)
					{
						float y = m_beginOffset - m_position + m_cursorPosition;
						renderer->drawLineX(0.0f, y, size4, ThemeConfig::RulerCursor);
					}
				}
			}

			void CRulerBar::onMouseClickLeft(float x, float y, bool down)
			{
				CBase::onMouseClickLeft(x, y, down);
				SPoint mousePosition(x, y);
				SPoint local = canvasPosToLocal(mousePosition);

				if (m_isHorizontal)
				{
					float valueX = local.X - m_beginOffset;
					float value = valueX / m_pixelPerUnit;

					if (OnMouseClickLeftValue != nullptr)
						OnMouseClickLeftValue(x, y, down, value);
				}
				else
				{
					float valueY = local.Y - m_beginOffset;
					float value = valueY / m_pixelPerUnit;

					if (OnMouseClickLeftValue != nullptr)
						OnMouseClickLeftValue(x, y, down, value);
				}
			}

			void CRulerBar::onMouseClickRight(float x, float y, bool down)
			{
				CBase::onMouseClickRight(x, y, down);
				SPoint mousePosition(x, y);
				SPoint local = canvasPosToLocal(mousePosition);

				if (m_isHorizontal)
				{
					float valueX = local.X - m_beginOffset;
					float value = valueX / m_pixelPerUnit;

					if (OnMouseClickRightValue != nullptr)
						OnMouseClickRightValue(x, y, down, value);
				}
				else
				{
					float valueY = local.Y - m_beginOffset;
					float value = valueY / m_pixelPerUnit;

					if (OnMouseClickRightValue != nullptr)
						OnMouseClickRightValue(x, y, down, value);
				}
			}
		}
	}
}