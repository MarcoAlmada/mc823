package example.biblio;
	
import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

import java.util.ArrayList;
import java.util.Arrays;
	
public class Server implements Biblio {
	
    public Server() {}
    
    private ArrayList<String> ISBN = new ArrayList<String> ();
    private ArrayList<String> titulo = new ArrayList<String> ();
    private ArrayList<String> descricao = new ArrayList<String> ();
	private ArrayList<Integer> estoque = new ArrayList<Integer> ();
	private ArrayList<String> autor = new ArrayList<String> ();
	private ArrayList<String> editora = new ArrayList<String> ();
	private ArrayList<Integer> ano = new ArrayList<Integer> ();

	/*private void preparaDados(){
		Scanner sc = new Scanner(dados.txt);
		while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                System.out.println(line);
            }
            scanner.close();
	}*/

    public String listaISBN() {
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
