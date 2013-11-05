namespace Quoridor {

class IRunnable {
public:
    virtual ~IRunnable() {}
    virtual void run() = 0;
};

}  /* namespace Quoridor */
