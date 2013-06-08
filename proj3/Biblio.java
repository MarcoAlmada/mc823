package example.biblio;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Biblio extends Remote {
    String listaISBN() throws RemoteException;
    String retornaDescricao(String isbn) throws RemoteException;
    String retornaInfo(String isbn) throws RemoteException;
    String retornaTudo() throws RemoteException;
    void alteraEstoque(String isbn, int valor) throws RemoteException;
    int retornaEstoque(String isbn) throws RemoteException;
}
