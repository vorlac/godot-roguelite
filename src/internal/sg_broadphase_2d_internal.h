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

#ifndef SG_BROADPHASE_2D_INTERNAL_H
#define SG_BROADPHASE_2D_INTERNAL_H

#include <unordered_map>
#include <vector>

#include "sg_fixed_rect2_internal.h"
#include "sg_result_handler_internal.h"

class SGBroadphase2DInternal
{
public:
    struct HashKey
    {
        union
        {
            struct
            {
                int32_t x;
                int32_t y;
            };

            uint64_t key;
        };

        _FORCE_INLINE_ HashKey()
        {
        }

        _FORCE_INLINE_ HashKey(int32_t p_x, int32_t p_y)
        {
            x = p_x;
            y = p_y;
        }

        _FORCE_INLINE_ HashKey(uint64_t p_key)
        {
            key = p_key;
        }

        _FORCE_INLINE_ bool operator==(HashKey p_other) const
        {
            return key == p_other.key;
        }

        _FORCE_INLINE_ bool operator<(HashKey p_other) const
        {
            return key < p_other.key;
        }

        struct HashFunction
        {
            _FORCE_INLINE_ size_t operator()(const HashKey& key_) const
            {
                return key_.key;
            }
        };
    };

    struct Element
    {
        SGCollisionObject2DInternal* object;
        SGFixedRect2Internal bounds;
        HashKey from;
        HashKey to;
        uint64_t query_id;

        _FORCE_INLINE_ Element()
        {
            object = nullptr;
            query_id = 0;
        }
    };

    struct Cell
    {
        std::vector<Element*> elements;
    };

private:
    std::vector<Element*> elements;
    std::unordered_map<HashKey, Cell*, HashKey::HashFunction> cells;
    int cell_size;
    mutable uint64_t current_query_id;

    void _add_element_to_cells(Element* p_element);
    void _remove_element_from_cells(Element* p_element);
    void _clear_cells();

public:
    Element* create_element(SGCollisionObject2DInternal* p_object);
    void update_element(Element* p_element);
    void delete_element(Element* p_element);

    // p_type is really SGCollisionObject2DInternal::ObjectType, but I couldn't work out the
    // circulate dependencies.
    void find_nearby(const SGFixedRect2Internal& p_bounds,
                     SGResultHandlerInternal* p_result_handler, int p_type = 3) const;

    void set_cell_size(int p_cell_size);

    SGBroadphase2DInternal(int p_cell_size);
    ~SGBroadphase2DInternal();
};

#endif
