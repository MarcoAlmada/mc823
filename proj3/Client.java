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
	    
	    	//contagem de tempo
	    	long inicio = 0, fim = 0;
	    	double sec;
	    	
	    	if(opt == 1){
	    		inicio = System.nanoTime();
	    		response = stub.listaISBN();
	    		fim = System.nanoTime();
	    	}
	    	if(opt == 2){
	    		String isbn = sc.next();
	    		inicio = System.nanoTime();
	    		response = stub.retornaDescricao(isbn);
	    		fim = System.nanoTime();
	    	}
	    	if(opt == 3){
	    		String isbn = sc.next();
	    		inicio = System.nanoTime();
	    		response = stub.retornaInfo(isbn);
	    		fim = System.nanoTime();
	    	}
	    	
	    	System.out.println(response);
	    	sec = fim-inicio;
	    	sec /= 1000000000;
	    	System.out.print("Tempo da operacao "+opt+": ");
	    	System.out.printf("%.9f\n", sec);
	    }
	    
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}
    }
}
