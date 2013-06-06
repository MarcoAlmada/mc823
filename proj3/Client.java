package example.biblio;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class Client {

    private Client() {}

    public static void main(String[] args) {

	String host = (args.length < 1) ? null : args[0];
	try {
	    Registry registry = LocateRegistry.getRegistry(host);
	    Biblio stub = (Biblio) registry.lookup("Biblio");
	    
	    //loop de leitura do teclado e processamento das requisicoes
	    while(true){
	 		Scanner sc = new Scanner(System.in);
	    	int opt = sc.nextInt();
	    	
	    	String response = new String(); //resposta do servidor
	    
	    	if(opt == 1){
	    		response = stub.listaISBN();
	    	}
	    	if(opt == 2){
	    		int ISBN = sc.nextInt();
	    		response = stub.retornaDescricao(ISBN);
	    	}
	    	if(opt == 3){
	    		int ISBN = sc.nextInt();
	    		response = stub.retornaInfo(ISBN);
	    	}
	    
	    	System.out.println("response: " + response);
	    	
	    }
	    
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}
    }
}
