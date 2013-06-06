package example.biblio;
	
import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

import java.util.ArrayList;
import java.util.Arrays;
	
public class Server implements Biblio {
	
    public Server() {}
    
    //private ArrayList<String>  = new ArrayList<String> ();

	//private void preparaDados(){
	
	
	//}

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
