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

#include "sg_bodies_2d_internal.h"
#include "sg_broadphase_2d_internal.h"
#include "sg_utils_internal.h"

void SGBroadphase2DInternal::_add_element_to_cells(SGBroadphase2DInternal::Element* p_element)
{
    HashKey from = p_element->from;
    HashKey to = p_element->to;

    for (int32_t x = from.x; x <= to.x; x++)
    {
        for (int32_t y = from.y; y <= to.y; y++)
        {
            HashKey key(x, y);
            auto cell_iter = cells.find(key);
            Cell* cell;

            if (cell_iter != cells.end())
            {
                cell = cell_iter->second;
            }
            else
            {
                cell = new Cell();
                cells.insert(std::make_pair(key, cell));
            }

            cell->elements.push_back(p_element);
        }
    }
}

void SGBroadphase2DInternal::_remove_element_from_cells(SGBroadphase2DInternal::Element* p_element)
{
    HashKey from = p_element->from;
    HashKey to = p_element->to;

    for (int32_t x = from.x; x <= to.x; x++)
    {
        for (int32_t y = from.y; y <= to.y; y++)
        {
            HashKey key(x, y);
            auto cell_iter = cells.find(key);

            if (cell_iter == cells.end())
                continue;

            Cell* cell = cell_iter->second;
            sg_remove_by_value(cell->elements, p_element);

            if (cell->elements.size() == 0)
            {
                cells.erase(key);
                delete cell;
            }
        }
    }
}

void SGBroadphase2DInternal::_clear_cells()
{
    for (auto i : cells)
        delete i.second;
    cells.clear();
}

SGBroadphase2DInternal::Element* SGBroadphase2DInternal::create_element(
    SGCollisionObject2DInternal* p_object)
{
    SGBroadphase2DInternal::Element* element = new SGBroadphase2DInternal::Element();
    elements.push_back(element);

    element->object = p_object;
    element->bounds = p_object->get_bounds();

    SGFixedVector2Internal min = element->bounds.get_min();
    SGFixedVector2Internal max = element->bounds.get_max();

    element->from = HashKey(min.x.to_int() / cell_size, min.y.to_int() / cell_size);
    element->to = HashKey(max.x.to_int() / cell_size, max.y.to_int() / cell_size);

    _add_element_to_cells(element);

    return element;
}

void SGBroadphase2DInternal::update_element(SGBroadphase2DInternal::Element* p_element)
{
    p_element->bounds = p_element->object->get_bounds();

    SGFixedVector2Internal min = p_element->bounds.get_min();
    SGFixedVector2Internal max = p_element->bounds.get_max();

    HashKey from(min.x.to_int() / cell_size, min.y.to_int() / cell_size);
    HashKey to(max.x.to_int() / cell_size, max.y.to_int() / cell_size);

    if (p_element->from == from && p_element->to == to)
        return;

    _remove_element_from_cells(p_element);

    p_element->from = from;
    p_element->to = to;

    _add_element_to_cells(p_element);
}

void SGBroadphase2DInternal::delete_element(SGBroadphase2DInternal::Element* p_element)
{
    _remove_element_from_cells(p_element);
    sg_remove_by_value(elements, p_element);
    delete p_element;
}

void SGBroadphase2DInternal::find_nearby(const SGFixedRect2Internal& p_bounds,
                                         SGResultHandlerInternal* p_result_handler, int p_type) const
{
    SGFixedVector2Internal min = p_bounds.get_min();
    SGFixedVector2Internal max = p_bounds.get_max();

    HashKey from(min.x.to_int() / cell_size, min.y.to_int() / cell_size);
    HashKey to(max.x.to_int() / cell_size, max.y.to_int() / cell_size);

    uint64_t query_id = (++current_query_id);

    for (int32_t x = from.x; x <= to.x; x++)
    {
        for (int32_t y = from.y; y <= to.y; y++)
        {
            HashKey key(x, y);
            auto cell_iter = cells.find(key);

            if (cell_iter == cells.end())
                continue;

            for (SGBroadphase2DInternal::Element* element : cell_iter->second->elements)
            {
                if (element->query_id == query_id)
                    continue;
                if ((element->object->get_object_type() & p_type) &&
                    p_bounds.intersects(element->bounds))
                {
                    element->query_id = query_id;
                    p_result_handler->handle_result(element->object, nullptr);
                }
            }
        }
    }
}

void SGBroadphase2DInternal::set_cell_size(int p_cell_size)
{
    if (cell_size != p_cell_size)
    {
        cell_size = p_cell_size;

        _clear_cells();
        for (SGBroadphase2DInternal::Element* element : elements)
        {
            SGFixedVector2Internal min = element->bounds.get_min();
            SGFixedVector2Internal max = element->bounds.get_max();

            element->from = HashKey(min.x.to_int() / cell_size, min.y.to_int() / cell_size);
            element->to = HashKey(max.x.to_int() / cell_size, max.y.to_int() / cell_size);

            _add_element_to_cells(element);
        }
    }
}

SGBroadphase2DInternal::SGBroadphase2DInternal(int p_cell_size)
{
    cell_size = p_cell_size;
    current_query_id = 0;
}

SGBroadphase2DInternal::~SGBroadphase2DInternal()
{
    _clear_cells();
    for (SGBroadphase2DInternal::Element* element : elements)
        delete element;
}
