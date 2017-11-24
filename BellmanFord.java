package bapspatil;

import java.util.Scanner;

public class BellmanFord {
	private int[] D;
	private int vertices;
	public static final int MAX_VALUE = 999;
	
	public BellmanFord(int vertices) {
		this.vertices = vertices;
		D = new int[vertices + 1];
	}
	
	public void bellmanFordEval(int source, int[][] A) {
		for(int node = 1; node <= vertices; node++)
			D[node] = MAX_VALUE;
		D[source] = 0;
		for(int node = 1; node <= vertices - 1; node++)
			for(int sn = 1; sn <= vertices; sn++)
				for(int dn = 1; dn <= vertices; dn++)
					if(A[sn][dn] != MAX_VALUE)
						D[dn] = Math.min(D[dn], D[sn] + A[sn][dn]);
		
		for(int sn = 1; sn <= vertices; sn++)
			for(int dn = 1; dn <= vertices; dn++)
				if(A[sn][dn] != MAX_VALUE) {
					System.out.println("Graph contains negative edge cycle.");
					break;
				}
		
		for(int vertex = 1; vertex <= vertices; vertex++)
			System.out.println("Distance of source " + source + " to " + vertex + " is " + D[vertex]);
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		
		System.out.println("Enter the number of vertices: ");
		int mVertices = in.nextInt();
		
		System.out.println("Enter the adjacency matrix: ");
		int[][] A = new int[mVertices + 1][mVertices + 1];
		for(int sn = 1; sn <= mVertices; sn++)
			for(int dn = 1; dn <= mVertices; dn++) {
				A[sn][dn] = in.nextInt();
				if(sn == dn) {
					A[sn][dn] = 0;
					continue;
				}
				if(A[sn][dn] == 0)
					A[sn][dn] = MAX_VALUE;
			}
		
		System.out.println("Enter the source vertex: ");
		int source = in.nextInt();
		
		BellmanFord b = new BellmanFord(mVertices);
		b.bellmanFordEval(source, A);
		in.close();
	}

}
