package example.biblio;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Biblio extends Remote {
    String listaISBN() throws RemoteException;
    String retornaDescricao(int ISBN) throws RemoteException;
    String retornaInfo(int ISBN) throws RemoteException;
}
