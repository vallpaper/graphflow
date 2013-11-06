#include <cstdlib>
#include <cstdio>
#include <climits>
#include <queue>
#include <map>

struct edge_capacity
{
	edge_capacity ( int tmp_max )
	{
		s_max = s_free = tmp_max;
		s_back = 0;
	}
	void add_flow ( int tmp_flow )
	{
		s_free -= tmp_flow;
		//s_back += tmp_flow;
	}
	void del_flow ( int tmp_flow )
	{
		s_back -= tmp_flow;
	}

	int s_max;
	int s_free;
	int s_back;
};

class graf_class
{
public:
	graf_class ( const char * file_name );
	~graf_class ( void );

	int nodes	( void ) { return c_nodes_int; };
	int start	( void ) { return c_start; };
	int end		( void ) { return c_end; };

	void print ( void );

	/* nebude implementováno - musel bych mít seznam všech hran a je 
		nějak očíslovat
	int edge_max_cap	( int edge );
	int edge_free_cap	( int edge );
	int edge_used_cap	( int edge );
	*/
	int max_cap		( int u, int v );
	int free_cap	( int u, int v );
	int used_cap	( int u, int v );

	/* funkce pro nastavení kapacity hrany */
	void add_flow	( int u, int v, int flow );

	/* tohle by mělo vracet iterator skrz mapu všech sousedů uzlu */
	// TODO
	std::map<int, struct edge_capacity> * neighbors	( int node );

private:
	std::map< int, std::map< int, struct edge_capacity > * > c_nodes;
	std::map< int, std::map< int, struct edge_capacity > * >::iterator c_it;
	std::map< int, struct edge_capacity >::iterator c_it_edge;

	int c_nodes_int;
	int c_start;
	int c_end;
};

graf_class::graf_class( const char * file_name )
{
	// načti a zpracuj zadaný vstupní soubor

	// jsme si jisti že ho načte
	FILE * f_input = fopen ( file_name, "r" );

	int tmp = 0;
	fscanf ( f_input, "%d\n", &tmp );
	c_nodes_int = tmp;
	c_start = 1;
	c_end = c_nodes_int;

	//printf("Nodes: %d\n", c_nodes_int );

	// Nacítání souboru
	for ( int i = 1; i <= c_nodes_int; i ++ )
	{
		std::map< int, struct edge_capacity > * tmp_nodes = new std::map< int, struct edge_capacity>;

		for ( int j = 1; j <= c_nodes_int; j ++ )
		{
			fscanf ( f_input, "%d", &tmp );

			if ( tmp != 0 )
				tmp_nodes -> insert ( std::pair<int, struct edge_capacity>( j, edge_capacity( tmp ) ) );
		}

		c_nodes . insert ( std::pair< int, std::map<int, struct edge_capacity> * >( i, tmp_nodes ) );
	}
	/*
	std::map< int, std::map< int, struct edge_capacity > * >::iterator it_1;
	std::map< int, struct edge_capacity >::iterator it_2;

	for ( it_1 = c_nodes . begin (); it_1 != c_nodes . end(); it_1 ++ )
	{
		for ( it_2 = it_1 -> second -> begin(); it_2 != it_1 -> second -> end(); it_2 ++ )
		{
			c_nodes . find ( it_2 -> first ) -> second -> insert ( std::pair<int, struct edge_capacity>( it_1 -> first, edge_capacity( it_2 -> second . s_max ) ) );
		}
	}
	*/
	fclose ( f_input );

}

graf_class::~graf_class ( void )
{
	std::map< int, std::map<int, struct edge_capacity> * >::iterator it;

	for ( it = c_nodes . begin(); it != c_nodes . end(); it++ )
		delete it -> second;
}

void graf_class::print ( void )
{
	std::map< int, std::map< int, struct edge_capacity > * >::iterator it_1;
	std::map< int, struct edge_capacity >::iterator it_2;

	for ( it_1 = c_nodes . begin (); it_1 != c_nodes . end(); it_1 ++ )
	{
		printf("[%d]:", it_1 -> first );
		for ( it_2 = it_1 -> second -> begin(); it_2 != it_1 -> second -> end(); it_2 ++ )
		{
			printf("\t%d - m:%d|f:%d|u:%d", it_2 -> first, it_2 -> second . s_max, it_2 -> second . s_free, it_2 -> second . s_back);
		}
		printf("\n");
	}
	printf("\n");

}

int graf_class::max_cap		( int u, int v )
{
	c_it = c_nodes . find ( u );
	if ( c_it == c_nodes . end() )
		return -1;
	else
	{
		c_it_edge = c_it -> second -> find ( v );
		if ( c_it_edge == c_it -> second -> end() )
			return -1;
		else
			return c_it_edge -> second . s_max;
	}

	/*
	c_it = c_nodes . find ( u );
	c_it_edge = c_it -> second -> find ( v );
	return c_it_edge -> second . s_max;
	*/
}

int graf_class::free_cap	( int u, int v ) {
	c_it = c_nodes . find ( u );
	if ( c_it == c_nodes . end() )
		return -1;
	else
	{
		c_it_edge = c_it -> second -> find ( v );
		if ( c_it_edge == c_it -> second -> end() )
			return -1;
		else
			return c_it_edge -> second . s_free;
	}
}

int graf_class::used_cap	( int u, int v ) {
	c_it = c_nodes . find ( u );
	if ( c_it == c_nodes . end() )
		return -1;
	else
	{
		c_it_edge = c_it -> second -> find ( v );
		if ( c_it_edge == c_it -> second -> end() )
			return -1;
		else
			return c_it_edge -> second . s_back;
	}
}

void graf_class::add_flow	( int u, int v, int flow ) {
	c_it = c_nodes . find ( u );
	if ( c_it == c_nodes . end() )
		return;
	else
	{
		c_it_edge = c_it -> second -> find ( v );
		if ( c_it_edge == c_it -> second -> end() )
			return;
		else
		{
			// pridání zpáteční cesty
			if ( c_nodes . find ( c_it_edge -> first ) -> second -> find ( c_it -> first ) == c_nodes . find ( c_it_edge -> first ) -> second -> end() )
				c_nodes . find ( c_it_edge -> first ) -> second -> insert ( std::pair<int, struct edge_capacity>( c_it -> first, edge_capacity( c_it_edge -> second . s_max ) ) );
			
			c_nodes . find ( c_it_edge -> first ) -> second -> find ( c_it -> first ) -> second . del_flow ( flow );
			return c_it_edge -> second . add_flow ( flow );
		}
	}
}

std::map<int, struct edge_capacity> * graf_class::neighbors	( int node )
{
	c_it = c_nodes . find ( node );
	return c_it -> second;
}



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
			//graph -> del_flow( v, u, flow );
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