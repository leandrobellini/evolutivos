#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define DIJ_MAX_PATHS 10
#define INFINITY 10000

typedef struct st_node node_t;
typedef struct st_path path_t;

typedef struct st_path {
	node_t * node;
	int distance;
	bool best;
} path_t;

typedef struct st_node {
	int distance;
	char name[10];
	path_t paths[DIJ_MAX_PATHS];
	bool visited;
} node_t;

void add_path_once( node_t * a, node_t * b, int distance ) {
	path_t * path = a->paths;
	while( path->node )
		path++;
	path->node = b;
	path->distance = distance;
}

void add_path( node_t * a, node_t * b, int distance ) {
	add_path_once( a, b, distance );
	add_path_once( b, a, distance );
}

void node_mark_best_path( node_t * node, path_t * m ) {
	path_t * p = node->paths;
	for( p = node->paths ; p->node ; p++ )
		p->best = (p == m);
}

void nodes_list( node_t ** nodes ) {
	node_t * n;
	while( (n = *nodes) ) {
		printf("* %s [%d] %s\n", n->name, n->distance, n->visited ? "visited" : "-" );
		path_t *p;
		for( p = n->paths; p->node; p++ )
			printf("  --(%d)--> %s %s\n", p->distance, p->node->name, p->best ? "[BEST]" : "" );
		nodes++;
	}
}

int dijkstra_node_distance( node_t * node ) {
	path_t * p;

	if ( ! node->visited ) {
		printf("dijkstra_node_distance( %s )...\n", node->name );
		node->visited = true;
		for( p = node->paths; p->node; p++) {
			int d = dijkstra_node_distance( p->node ) + p->distance ;
			if ( d < node->distance ) {
				node->distance = d;
				node_mark_best_path( node, p );
			}
		}
		printf("dijkstra_node_distance( %s ): %d\n", node->name, node->distance );
	}

	return node->distance;
}

void dijskstra_nodes( node_t ** nodes ) {
	node_t * n;
	while( (n = *nodes) ) {
		dijkstra_node_distance( n );
		nodes++;
	}
}

void dijkstra_solution( node_t * node ) {
	printf("%s[%d]", node->name, node->distance );

	path_t * p;
	for( p = node->paths; p->node; p++ ) {
		if ( p->best ) {
			printf(" --(%d)--> ", p->distance );
			dijkstra_solution( p->node );
			break;
		}
	}
}

int main() {

	node_t
	a = {distance:  0, name: "a"},
	b = {distance: INFINITY, name: "b"},
	c = {distance: INFINITY, name: "c"},
	d = {distance: INFINITY, name: "d"},
	e = {distance: INFINITY, name: "e"},
	f = {distance: INFINITY, name: "f"},
	g = {distance: INFINITY, name: "g"},
	h = {distance: INFINITY, name: "h"},
	i = {distance: INFINITY, name: "i"};

	add_path( & a, & b, 7 );
	add_path( & a, & c, 4 );
	add_path( & a, & d, 5 );
	add_path( & b, & c, 2 );
	add_path( & b, & e, 25 );
	add_path( & e, & g, 10 );
	add_path( & g, & i, 2 );
	add_path( & c, & h, 9 );
	add_path( & h, & i, 3 );
	add_path( & d, & f, 9 );
	add_path( & f, & h, 20 );

	node_t * nodes [] = { & a, & b, & c, &d, & e, & f, & g, & h, & i, NULL };

	node_t * target;

	target = &i;

	dijkstra_node_distance( target );

		nodes_list( nodes );

		printf("Solution for %s: ", target->name);
		dijkstra_solution( target );
		printf("\n");



	return 0;
}
