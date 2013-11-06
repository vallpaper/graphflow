#include "graf_class.hpp"
#include <cstdlib>
#include <cstdio>
#include <climits>
#include <queue>

graf_class * graph = NULL;

// TODO: predávané argumenty
int breadth_first_search ( int * parent_table )
{
	int cap_flow[ graph -> nodes() + 1 ];

	for ( int i = 1; i <= graph -> nodes(); i ++ )
		parent_table [ i ] = -1;
	parent_table [ graph -> start() ] = -2;

	cap_flow [ graph -> start() ] = INT_MAX;

	std::queue<int> m_Q;
	m_Q . push ( graph -> start() );

	while ( m_Q . size() > 0 )
	{
		int u = m_Q . front();
		m_Q . pop();

		std::map< int, struct edge_capacity > * neighbors = graph -> neighbors ( u );
		std::map< int, struct edge_capacity >::iterator it;

		for ( it = neighbors -> begin(); it != neighbors -> end(); it ++ )
		{
			// stacilo by it -> second . s_free; -> budoucí optimalizace
			//if ( graph -> free_cap ( u, it -> first ) > 0 && parent_table[ it -> first ] == -1 )
			if ( it -> second . s_free > 0 && parent_table[ it -> first ] == -1 )
			{
				parent_table[ it -> first ] = u;
				cap_flow[ it -> first ] = ( cap_flow[ u ] < it -> second . s_free ) ? cap_flow[ u ] : it -> second . s_free;

				if ( it -> first != graph -> end() )
				{
					m_Q . push ( it -> first );
				}
				else
				{
					return cap_flow[ graph -> end() ]; // TODO: it -> first
				}
			}
		}
	}
	return 0;
/*
   input:
       C, E, s, t
   output:
       M[t]          (Kapacita nalezené cesty)
       P             (Parent table)
   P := array(1..n)
   for u in 1..n
       P[u] := -1
   P[s] := -2 (ujistěte se, že zdroj není objeven podruhé)
   M := array(1..n) (Kapacita nalezené cesty k vrcholu)
   M[s] := &infin
   Q := queue()
   Q.push(s)
   while Q.size() > 0
       u := Q.pop()
       for v in E[u]
           (Jestli je dostupná kapacita a v ještě nebylo nalezené)
           if C[u,v] - F[u,v] > 0 and P[v] = -1
               P[v] := u
               M[v] := min(M[u], C[u,v] - F[u,v])
               if v ≠ t
                   Q.push(v)
               else
                   return M[t], P
   return 0, P

	*/
}


int edm_karp ( void )
{
	int max_flow = 0;
	int parent_table[ graph -> nodes() + 1 ];


	while ( true )
	{
		int flow = breadth_first_search( parent_table );

		if ( flow == 0 )
			break;

		max_flow += flow;

		int v = graph -> end();

		while ( v != graph -> start() )
		{
			int u = parent_table[ v ];
			graph -> add_flow( u, v, flow );
			v = u;
		}
		//graph -> print();
	}
/*
   forever
       m, P := BreadthFirstSearch(C, E, s, t)
       if m = 0
           break
       f := f + m
       (Vyhledává backtrackingem a vypisuje tok)
       v := t
       while v ≠ s
           u := P[v]
           F[u,v] := F[u,v] + m
           F[v,u] := F[v,u] - m
           v := u
   return (f, F)

*/

   return max_flow;
}


int main(int argc, char const *argv[])
{

	graph = new graf_class ( argv[1] );
	//graph -> print();
	/* input:
	 */
	int max_flow = edm_karp();

	printf("MAX FLOW:\t%d\n", max_flow );

	return 0;
}