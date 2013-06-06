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
	
    public Server() {}
    
    private ArrayList<String> ISBN = new ArrayList<String> ();
    private ArrayList<String> titulo = new ArrayList<String> ();
    private ArrayList<String> descricao = new ArrayList<String> ();
	private ArrayList<Integer> estoque = new ArrayList<Integer> ();
	private ArrayList<String> autor = new ArrayList<String> ();
	private ArrayList<String> editora = new ArrayList<String> ();
	private ArrayList<Integer> ano = new ArrayList<Integer> ();

	private void preparaDados(){
		File file = new File("dados.txt");
		try{
			Scanner sc = new Scanner(file);
			String line = new String();
			while (sc.hasNextLine()) {
		        line = sc.nextLine();
		        ISBN.add(line);
		        line = sc.nextLine();
		        titulo.add(line);
		        line = sc.nextLine();
		        descricao.add(line);
		        line = sc.nextLine();
		        estoque.add(Integer.parseInt(line));
		        line = sc.nextLine();
		        autor.add(line);
		        line = sc.nextLine();
		        editora.add(line);            
		        line = sc.nextLine();
		        ano.add(Integer.parseInt(line));                
		    }
		    System.out.println( Arrays.toString(ISBN.toArray()));
		    System.out.println( Arrays.toString(titulo.toArray()));
		    System.out.println( Arrays.toString(descricao.toArray()));
		    System.out.println( Arrays.toString(estoque.toArray()));
		    System.out.println( Arrays.toString(autor.toArray()));
		    System.out.println( Arrays.toString(editora.toArray()));
		    System.out.println( Arrays.toString(ano.toArray()));
		    sc.close();
		} catch (FileNotFoundException e) {
            e.printStackTrace();
        }
	}

    public String listaISBN() {
    preparaDados();
	return "Lista ISBN";
    }
    
    public String retornaDescricao(int ISBN) {
	return "Retorna Descricao " + ISBN;
    }
    
    public String retornaInfo(int ISBN) {
	return "Retorna info " + ISBN;
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
