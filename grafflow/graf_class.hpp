#ifndef __HPP_GRAF_CLASS_6546546546456__
#define __HPP_GRAF_CLASS_6546546546456__
#include <cstdlib>
#include <cstdio>
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
		s_back += tmp_flow;
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
			c_nodes . find ( c_it_edge -> first ) -> second -> insert ( std::pair<int, struct edge_capacity>( c_it -> first, edge_capacity( c_it_edge -> second . s_max ) ) );
			c_nodes . find ( c_it_edge -> first ) -> second -> find ( c_it -> first ) -> second . add_flow ( flow );
			return c_it_edge -> second . add_flow ( flow );
		}
	}
}

std::map<int, struct edge_capacity> * graf_class::neighbors	( int node )
{
	c_it = c_nodes . find ( node );
	return c_it -> second;
}

#endif