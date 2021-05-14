#ifndef INPUT_H
#define INPUT_H

class Input {
public:
    Input take();

    bool any() const { return m_any; }
    bool forward() const { return m_forward && m_forward_is_current; }
    bool back() const { return m_back && !m_forward_is_current; }
    bool left() const { return m_left && m_left_is_current; }
    bool right() const { return m_right && !m_left_is_current; }
    bool mouse_click() const { return m_mouse; }
    int mouse_delta_x() const { return m_delta_x; }
    int mouse_delta_y() const { return m_delta_y; }

    void handle_key(int key, bool down);
    void handle_mouse_click(bool down);
    void handle_mouse_move(int delta_x, int delta_y);

private:
    bool m_any { false };
    bool m_forward { false };
    bool m_back { false };
    bool m_forward_is_current { false };
    bool m_left { false };
    bool m_right { false };
    bool m_left_is_current { false };
    bool m_mouse { false };
    int m_delta_x { 0 };
    int m_delta_y { 0 };
};

#endif
