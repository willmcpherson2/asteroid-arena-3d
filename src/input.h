#ifndef INPUT_H
#define INPUT_H

class Input {
public:
    Input take();

    bool any() const { return m_any; }
    bool forward() const { return m_forward; }
    int mouse_delta_x() const { return m_delta_x; }
    int mouse_delta_y() const { return m_delta_y; }

    void handle_key(int key, bool down);
    void handle_mouse_move(int delta_x, int delta_y);

private:
    bool m_any { false };
    bool m_forward { false };
    int m_delta_x { 0 };
    int m_delta_y { 0 };
};

#endif
