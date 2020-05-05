#if HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef WIN32
#pragma warning(disable : 4786)   /* Disable annoying warning messages */
#endif

#include "CFGEdge.h"
#include "Statement.h"


CFGEdge::CFGEdge(const Statement* src, const Statement* dest, bool post_dest, bool back_link)
    : src(src),
      dest(dest),
	  post_dest(post_dest),
	  back_link(back_link)
{
	// nothing else to do
}

CFGEdge::CFGEdge(const CFGEdge &edge)
	: src(edge.src),
	  dest(edge.dest),
	  post_dest(edge.post_dest),
	  back_link(edge.back_link)
{
	// nothing else to do
}

CFGEdge::~CFGEdge(void)
{
}

///////////////////////////////////////////////////////////////////////////////

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
