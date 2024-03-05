#ifndef LWE__D3D_SPACE__COMMAND_HPP__
#define LWE__D3D_SPACE__COMMAND_HPP__

class ICommand
{
public:
    virtual void Execute() = 0;
};

#endif