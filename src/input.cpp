#include "input.h"
#include "parameters.h"
#include <cstdlib>
#include <iostream>

Input Input::take()
{
    Input input = *this;

    m_delta_x = 0;
    m_delta_y = 0;

    return input;
}

void Input::handle_key(int key, bool down)
{
    m_any = down;

    switch (key) {
    case parameters::input::forward:
        m_forward = down;
        m_forward_is_current = down;
        break;
    case parameters::input::back:
        m_back = down;
        m_forward_is_current = !down;
        break;
    case parameters::input::left:
        m_left = down;
        m_left_is_current = down;
        break;
    case parameters::input::right:
        m_right = down;
        m_left_is_current = !down;
        break;
    case 'q':
    case 27:
        std::exit(EXIT_SUCCESS);
    }
}

void Input::handle_mouse_click(bool down)
{
    m_mouse = down;
}

void Input::handle_mouse_move(int delta_x, int delta_y)
{
    // Flip screen y-direction to world y-direction
    delta_y = -delta_y;

    m_delta_x += delta_x;
    m_delta_y += delta_y;
}
