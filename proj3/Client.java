package example.biblio;

import example.biblio.Biblio;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;
import java.util.InputMismatchException;

public class Client {

    private Client() {}

    public static void main(String[] args) {

	String host = (args.length < 1) ? null : args[0];
	try {
	    Registry registry = LocateRegistry.getRegistry(host);
	    Biblio stub = (Biblio) registry.lookup("Biblio");
            
	    //loop de leitura do teclado e processamento das requisicoes
	    while(true){
                System.out.println("1- listar todos os ISBN e seus respectivos titulos");
                System.out.println("2- dado o ISBN de um livro, retornar descricao");
                System.out.println("3- dado o ISBN de um livro, retornar todas as informacoes do livro");
                System.out.println("4- listar todas as informacoes de todos os livros");
                System.out.println("5- alterar numero de exemplares em estoque");
                System.out.println("6- dado o ISBN de um livro, retornar numero de exemplares em estoque");
                System.out.println("7- fechar cliente");

                Scanner sc = new Scanner(System.in);
                int opt = 0;
                try{
                    System.out.println("Digite o numero da operacao");
                    opt = sc.nextInt();
                } catch (InputMismatchException e) {
                    System.out.println("Digite o numero da operacao");
                }
	    	
	    	String response = new String(); //resposta do servidor
	    
	    	//contagem de tempo
	    	long inicio = 0, fim = 0;
	    	double sec;
	    	
	    	if(opt == 1){
                    //contagem de tempo e chamada
                    inicio = System.nanoTime();
                    response = stub.listaISBN();
                    fim = System.nanoTime();
	    	}
	    	if(opt == 2){
                    System.out.println("Digite o ISBN do livro");
                    String isbn = sc.next();
                    //contagem de tempo e chamada
                    inicio = System.nanoTime();
                    response = stub.retornaDescricao(isbn);
                    fim = System.nanoTime();
	    	}
                
	    	if(opt == 3){
                    System.out.println("Digite o ISBN do livro");
                    String isbn = sc.next();
                    //contagem de tempo e chamada
                    inicio = System.nanoTime();
                    response = stub.retornaInfo(isbn);
                    fim = System.nanoTime();
	    	}

                if(opt == 4){
                    //contagem de tempo e chamada
                    inicio = System.nanoTime();
                    response = stub.retornaTudo();
                    fim = System.nanoTime();
                }
                
                if(opt == 5){
                    System.out.println("Digite a senha");
                    String senha = sc.next(); // verifica se cliente tem autorizacao
                    if(senha.equals("senhalivraria")){
                        System.out.println("Digite o ISBN do livro");
                        String isbn = sc.next();
                        System.out.println("Informe o novo valor do estoque");
                        int valor = sc.nextInt();
                        //contagem de tempo e chamada
                        inicio = System.nanoTime();
                        stub.alteraEstoque(isbn, valor);
                        fim = System.nanoTime();
                    }
                    else {
                        System.out.println("Senha invalida.");
                    }
                }

                if(opt == 6){
                    System.out.println("Digite o ISBN do livro");
                    String isbn = sc.next();
                    //contagem de tempo e chamada
                    inicio = System.nanoTime();
                    response += stub.retornaEstoque(isbn);
                    fim = System.nanoTime();
                }
                
                if(opt == 7){
                    System.exit(0);
                }
	    	
	    	System.out.println(response);
	    	sec = fim-inicio;
	    	sec /= 1000000000;
	    	System.out.print("Tempo da operacao: ");
	    	System.out.printf("%.9f\n", sec);
	    }
            
	    
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}
    }
}
