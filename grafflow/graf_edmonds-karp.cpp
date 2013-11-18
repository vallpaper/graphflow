#include <cstdlib>
#include <cstdio>
#include <climits>
#include <queue>

int ** graph_orig = NULL;
int ** graph_flow = NULL;
int graph_nodes;
int graph_source = 0;
int graph_sink;

void loadGraph ( const char * file_name )
{
	FILE * f_input = fopen ( file_name, "r" );

	int tmp = 0;

	// nacteni velikosti grafu
	fscanf ( f_input ,"%d\n", &tmp );
	graph_nodes = tmp;
	graph_sink = tmp - 1;
	
	graph_orig = new int * [ graph_nodes ];
	graph_flow = new int * [ graph_nodes ];
	// vytvoření matice pro graf
	for ( int i = 0; i < graph_nodes; i ++ )
	{
		graph_orig[ i ] = new int [ graph_nodes ];
		graph_flow[ i ] = new int [ graph_nodes ];
	}

	for ( int i = 0; i < graph_nodes; i ++ )
	{
		for ( int j = 0; j < graph_nodes; j ++ )
		{
			fscanf ( f_input, "%d", &tmp );
			graph_orig [ i ] [ j ] = tmp;
			graph_flow [ i ] [ j ] = 0;
		}
	}
}

int breadth_first_search ( int * parent_table )
{
	int flow = 0;

	for ( int i = 0; i < graph_nodes; i ++ )
		parent_table [ i ] = -1;
	parent_table [ graph_source ] = -2;

	int path_capacity [ graph_nodes ];
	for ( int i = 0; i < graph_nodes; i ++)
		path_capacity [ i ] = INT_MAX;

	std::queue<int> m_Q;
	m_Q . push ( graph_source );

	while ( m_Q . size() > 0 )
	{
		int u = m_Q . front();
		m_Q . pop();

		for ( int v = 0; v < graph_nodes; v ++ )
		{
			if ( graph_flow [ v ] == 0 ) continue;

			if ( graph_orig [u ] [ v ] - graph_flow [ u ] [ v ] > 0 && parent_table [ v ] == -1 )
			{
				parent_table [ v ] = u;
				path_capacity [ v ] = path_capacity [ u ] < ( graph_orig [u ] [ v ] - graph_flow [ u ] [ v ] ) ? path_capacity [ u ] : ( graph_orig [u ] [ v ] - graph_flow [ u ] [ v ] );

				if ( v != graph_sink )
					m_Q . push ( v );
				else
					return path_capacity [ graph_sink ];

			}
		}
	}

	return flow;
}

int edmonds_karp ( void )
{
	int max_flow = 0;
	int flow;
	int parent_table [ graph_nodes ];

	while ( true )
	{
		flow = breadth_first_search ( parent_table );

		if ( flow == 0 )
			break;

		max_flow += flow;

		int v = graph_sink;
		while ( v != graph_source )
		{
			int u = parent_table [ v ];
			graph_flow [ u ] [ v ] += flow;
			graph_flow [ v ] [ u ] -= flow;
			v = u;
		}
	}

	return max_flow;
}


int main(int argc, char const *argv[])
{
	if ( argc < 2 ) {
		printf("Input file missing!\n");
	} else {
		loadGraph ( argv [ 1 ] );
		int max_flow = edmonds_karp();
		printf("Flow:\t%d\n", max_flow );
	}
	return 0;
}