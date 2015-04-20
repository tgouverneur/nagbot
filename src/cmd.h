#ifndef CMD_H
#define CMD_H

template<class O>
struct Cmd
{
        std::string     token;
        bool            (O::*f)(Message&);

        Cmd(std::string t, bool(O::*fct)(Message&)) : token(t), f(fct) { };
};

#endif
