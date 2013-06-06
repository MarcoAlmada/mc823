package example.biblio;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Biblio extends Remote {
    String listaISBN() throws RemoteException;
    String retornaDescricao(String isbn) throws RemoteException;
    String retornaInfo(String isbn) throws RemoteException;
}
