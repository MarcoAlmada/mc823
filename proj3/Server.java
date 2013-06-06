package example.biblio;
	
import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

import java.util.ArrayList;
import java.util.Arrays;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
	
public class Server implements Biblio {
	
    public Server() {
    	preparaDados();	
    }
    
    //ArrayLists armazenarao banco de dados da biblioteca
    private ArrayList<String> ISBN = new ArrayList<String> ();
    private ArrayList<String> titulo = new ArrayList<String> ();
    private ArrayList<String> descricao = new ArrayList<String> ();
	private ArrayList<Integer> estoque = new ArrayList<Integer> ();
	private ArrayList<String> autor = new ArrayList<String> ();
	private ArrayList<String> editora = new ArrayList<String> ();
	private ArrayList<Integer> ano = new ArrayList<Integer> ();
	
	//variaveis para contagem de tempo
	long inicio = 0, fim = 0;
	double sec;
	
	/*Pre-processamento: Os dados do banco serao armazenados na
	nas 7 ArrayList declaradas acima*/
	private void preparaDados(){
		File file = new File("dados.txt");
		try{
			Scanner sc = new Scanner(file);
			while (sc.hasNextLine()) {
		        ISBN.add(sc.nextLine());
		        titulo.add(sc.nextLine());
		        descricao.add(sc.nextLine());
		        estoque.add(Integer.parseInt(sc.nextLine()));
		        autor.add(sc.nextLine());
		        editora.add(sc.nextLine());            
				ano.add(Integer.parseInt(sc.nextLine()));                
		    }
		    sc.close();
		} catch (FileNotFoundException e) {
            e.printStackTrace();
        }
	}
	
	//Operacao 1
    public String listaISBN() {
    	//obtem tempo inicial
    	inicio = System.nanoTime();
    	
    	//processa requisicao
    	String response = Arrays.toString(ISBN.toArray());
    	
    	//obtem tempo final e imprime
    	fim = System.nanoTime();
		sec = fim-inicio;
	    sec /= 1000000000;
	    System.out.print("Tempo da operacao: ");
	    System.out.printf("%.9f\n", sec);

		return response;
    }
    
    //Operacao 2
    public String retornaDescricao(String isbn) {
    	//obtem tempo inicial
    	inicio = System.nanoTime();
    	
    	//processa requisicao
    	String response = new String();
		for (int i = 0; i < ISBN.size(); i++) {
			String str = ISBN.get(i);
			if(str.compareTo(isbn) == 0){
				response = descricao.get(i);
			}
		}
		
		//obtem tempo final e imprime
    	fim = System.nanoTime();
		sec = fim-inicio;
	    sec /= 1000000000;
	    System.out.print("Tempo da operacao: ");
	    System.out.printf("%.9f\n", sec);
	    
		return response;
    }
    
    //Operacao 3
    public String retornaInfo(String isbn) {
    	//obtem tempo inicial
    	inicio = System.nanoTime();
    	
    	//processa requisicao
		String response = new String();
		for (int i = 0; i < ISBN.size(); i++) {
			String str = ISBN.get(i);
			if(str.compareTo(isbn) == 0){
				response = isbn + "\n";
				response += titulo.get(i) + "\n";
				response += descricao.get(i) + "\n";
				response += estoque.get(i) + "\n";
				response += autor.get(i) + "\n";
				response += editora.get(i) + "\n";
				response += ano.get(i);
			}
		}
		
		//obtem tempo final e imprime 
    	fim = System.nanoTime();
		sec = fim-inicio;
	    sec /= 1000000000;
	    System.out.print("Tempo da operacao: ");
	    System.out.printf("%.9f\n", sec);
		
		return response;
    }
	
    public static void main(String args[]) {
	
	try {
	    Server obj = new Server();
	    Biblio stub = (Biblio) UnicastRemoteObject.exportObject(obj, 0);

	    // Bind the remote object's stub in the registry
	    Registry registry = LocateRegistry.getRegistry();
	    registry.bind("Biblio", stub);
	    System.err.println("Server ready");
	} catch (Exception e) {
	    System.err.println("Server exception: " + e.toString());
	    e.printStackTrace();
	}
    }
}
