/*************************************************************************/
/* Copyright (c) 2021-2022 David Snopek                                  */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include <godot_cpp/classes/engine.hpp>

#include "sg_fixed_position_2d.h"

#define DEFAULT_EXTENTS_FIXED 655360
#define DEFAULT_EXTENTS_FLOAT 10.0

void SGFixedPosition2D::_notification(int p_what)
{
    switch (p_what)
    {
        case NOTIFICATION_ENTER_TREE:
        {
            queue_redraw();
        }
        break;
        case NOTIFICATION_DRAW:
        {
            if (!is_inside_tree())
                break;
            if (Engine::get_singleton()->is_editor_hint())
            {
                draw_line(Point2(-DEFAULT_EXTENTS_FLOAT, 0), Point2(+DEFAULT_EXTENTS_FLOAT, 0),
                          Color(0.96, 0.20, 0.32));
                draw_line(Point2(0, -DEFAULT_EXTENTS_FLOAT), Point2(0, +DEFAULT_EXTENTS_FLOAT),
                          Color(0.53, 0.84, 0.01));
            }
        }
        break;
    }
}

void SGFixedPosition2D::_bind_methods()
{
}

SGFixedPosition2D::SGFixedPosition2D()
{
}
