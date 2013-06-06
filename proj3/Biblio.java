package example.biblio;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Biblio extends Remote {
    String listaISBN() throws RemoteException;
}
