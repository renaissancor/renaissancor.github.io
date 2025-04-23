
// -----------------------------------------------------------
// Class showing copy constructor and assignment operator
class CNew
{
private:
    int     m_i;
    short   m_s;
public:
    void SetInt(int _a) { m_i = _a; }

    // Default assignment operator
    CNew& operator=(const CNew& _Other)
    {
        m_i = _Other.m_i;
        m_s = _Other.m_s;
        return *this;
    }

public:
    CNew() : m_i(0), m_s(0) {}

    // Prevent default copy constructor (commented out here)
    // CNew(const CNew& _Other) = delete;

    // Custom copy constructor
    CNew(const CNew& _Other)
        : m_i(_Other.m_i)
        , m_s(_Other.m_s)
    {}
};

int main()
{
    // Default constructor
    CNew Inst1;
    Inst1.SetInt(10);

    // Copy constructor
    CNew Inst2(Inst1);

    // Assignment operator
    CNew Inst3;
    Inst2 = Inst3 = Inst1;

    // If you declare and assign in one line, compiler optimizes it as copy constructor
    CNew Inst4 = Inst3;

    int k = 100;
    int a = k;

    return 0;
}

