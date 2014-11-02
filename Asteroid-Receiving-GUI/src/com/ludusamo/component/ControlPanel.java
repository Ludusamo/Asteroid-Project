package com.ludusamo.component;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;

import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

import com.ludusamo.util.Util;

public class ControlPanel extends JPanel {
	JButton[] buttons;

	private static final String ALERT = "ATTENTION: Continuing will clear all data up to this point.";

	MainComponent mainC;

	Graphics g;
	
	public ControlPanel(MainComponent mainC) {
		this.mainC = mainC;
//		setLayout(new FlowLayout(FlowLayout.CENTER));
		setLayout(new GridBagLayout());
		addButtons();
		validate();
	}

	private void addButtons() {
		buttons = new JButton[4];

		buttons[0] = new JButton("Start");
		buttons[0].addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				Object[] options = { "OK", "CANCEL" };
				int decision = JOptionPane.showOptionDialog(null, ALERT,
						"Warning", JOptionPane.DEFAULT_OPTION,
						JOptionPane.WARNING_MESSAGE, null, options, options[0]);
				if (decision == 0)
					mainC.startCollectingData();
			}
		});

		buttons[1] = new JButton("Stop");
		buttons[1].addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				mainC.stopCollectingData();
			}
		});

		buttons[2] = new JButton("Continue");
		buttons[2].addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				mainC.continueCollectingData();
			}
		});

		buttons[3] = new JButton("Check");
		buttons[3].addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				mainC.evaluateData();
			}
		});

		// start.set
		GridBagConstraints c = new GridBagConstraints();
		for (int i = 0; i < buttons.length; i++) {
			c.gridwidth = 1;
			c.gridheight = 1;
			c.gridx = i % 2;
			c.gridy = i / 2;
			add(buttons[i], c);
		}
	}

	public void resize() {
		float width = .4f;
		float height = .4f;
		for (int i = 0; i < buttons.length; i++) {
			buttons[i].setPreferredSize(new Dimension((int) (getWidth() * width),
					(int) (getHeight() * height)));
			buttons[i].setSize(new Dimension((int) (getWidth() * width),
					(int) (getHeight() * height)));
		}
	}
	
	@SuppressWarnings("deprecation")
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		BufferedImage bg = MainComponent.backgroundImg;
		this.g = g;
		
		Util.drawBackground(this, MainComponent.backgroundImg, g);
	}
}
