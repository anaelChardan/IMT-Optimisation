import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

public class HeritageGUIKO {
	private String ceQuonVeutAfficher = "Coucou";
	public HeritageGUIKO() {
		JFrame window = new JFrame("Hello Button");
		JButton button = new JButton("Hello console");
		Container container = window.getContentPane();
		container.setLayout(new FlowLayout());
		container.add(button);
		JButton button2 = new JButton("Bonjour console");
		button.addActionListener(new ButtonListener());
		button2.addActionListener(new ButtonListener());
		container.add(button2);
		window.pack();
		window.setLocationRelativeTo(null);
		window.setVisible(true);
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	class ButtonListener implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			if( ((JButton) e.getSource()).getText().equals("Hello console") ) {
				System.out.println("Hi there ! (KO)");
				System.out.println(ceQuonVeutAfficher);
			} else
				System.out.println("Bonjour ! (KO)");
		}
	}

	public static void main(String[] args) {
		new HeritageGUIKO();
	}
}
