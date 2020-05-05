#ifndef CFG_EDGE_H
#define CFG_EDGE_H

#include <iostream>
#include <vector>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
class Statement;
class Block;

/*
 *
 */
class CFGEdge
{
public:
	CFGEdge(const Statement* src, const Statement* dest, bool post_dest, bool back_link);
	CFGEdge(const CFGEdge &edge);
	virtual ~CFGEdge(void);

	// void Output(std::ostream &out) const;

	const Statement* src;
	const Statement* dest;
	bool  post_dest;
	bool  back_link;
};

///////////////////////////////////////////////////////////////////////////////

#endif // CFG_EDGE_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
