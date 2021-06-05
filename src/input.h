#ifndef INPUT_H
#define INPUT_H

class Input {
public:
    Input take();

    bool fire() const { return m_fire; }
    bool forward() const { return m_forward; }
    bool left() const { return m_left; }
    bool right() const { return m_right; }
    bool back() const { return m_back; }
    int mouse_delta_x() const { return m_delta_x; }
    int mouse_delta_y() const { return m_delta_y; }

    void handle_key(int key, bool down);
    void handle_mouse_move(int delta_x, int delta_y);

private:
    bool m_fire { false };
    bool m_forward { false };
    bool m_left { false };
    bool m_right { false };
    bool m_back { false };
    int m_delta_x { 0 };
    int m_delta_y { 0 };
};

#endif
