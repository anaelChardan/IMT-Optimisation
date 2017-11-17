import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

public class HeritageGUIOK {
	private String ceQuonVeutAfficher = "Coucou";
	public HeritageGUIOK() {
		JFrame window = new JFrame("Hello Button");
		JButton button = new JButton("Hello console");
		Container container = window.getContentPane();
		container.setLayout(new FlowLayout());
		container.add(button);
		JButton button2 = new JButton("Bonjour console");
		button.addActionListener(new ButtonListener());
		button2.addActionListener(new Button2Listener());
		container.add(button2);
		window.pack();
		window.setLocationRelativeTo(null);
		window.setVisible(true);
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	class ButtonListener implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			System.out.println("Hi there !");
			System.out.println(ceQuonVeutAfficher);
		}
	}

	class Button2Listener implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			System.out.println("Bonjour !");
		}
	}

	public static void main(String[] args) {
		new HeritageGUIOK();
	}
}
