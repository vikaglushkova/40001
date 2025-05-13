#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <map>
#include <limits>
#include <string>

#include "processor.hpp"
#include "polygon.hpp"

namespace andriuschin
{
  template< class Processor >
  class Parser
  {
  public:
    using processor = Processor;
    using call_signature = bool(processor::*)(Context&);
    using map_type = std::map< std::string, call_signature >;
    Context context;
    template< class P = processor, class M = map_type >
    Parser(P&& proc, Context context, M&& comands):
      context(context),
      proccessor_(std::forward< P >(proc)),
      comands_(std::forward< M >(comands))
    {}
    bool run()
    {
      std::string command;
      if ((context.input >> command))
      {
        typename map_type::iterator current = comands_.find(command);
        if (current != comands_.end())
        {
          if ((proccessor_.*(current->second))(context))
          {
            return context.input.good();
          }
        }
        context.input.clear(context.input.rdstate() & ~std::ios::failbit);
        context.input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        context.output << "<INVALID COMMAND>\n";
      }
      return context.input.good();
    }
  private:
    processor proccessor_;
    map_type comands_;
  };
}

#endif
