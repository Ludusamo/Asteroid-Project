package com.ludusamo.component;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.image.BufferedImage;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import com.ludusamo.util.CommandInterpreter;
import com.ludusamo.util.Util;

public class ControlPanel extends JPanel {
	public static final int MIN_FONT_SIZE = 20;
	public static final int MAX_FONT_SIZE = 25;
	private Dimension minimumDimension;
	
	JTextField commandLine;
	CommandInterpreter comInt;
	JLabel outputLabel;
	
	private static final String ALERT = "ATTENTION: Continuing will clear all data up to this point.";

	MainComponent mainC;

	private void changeFontSize(JLabel l, int size) {
		Font f = l.getFont();
		l.setFont(f.deriveFont(f.getStyle(), size));
	}

	protected void adaptLabelFont(JLabel l) {
		float newSize = (minimumDimension.width / getSize().width)
				* (MAX_FONT_SIZE - MIN_FONT_SIZE);
		newSize = MAX_FONT_SIZE - newSize;

		changeFontSize(l, (int) newSize);

		l.repaint();
		revalidate();
	}
	
	public ControlPanel(MainComponent mainC) {
		super(new GridBagLayout());
		this.mainC = mainC;
		addCommandLine();
		validate();
	}

	private void addCommandLine() {
		commandLine = new JTextField();
		commandLine.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				comInt.processInput(commandLine.getText());
				commandLine.setText("");
			}
		});

		outputLabel = new JLabel("Output Text");
		outputLabel.setFont(MainComponent.font.deriveFont(12));
		outputLabel.setForeground(Color.white);
		comInt = new CommandInterpreter(this.mainC, outputLabel);
		
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 0;
		c.gridy = 0;
		c.gridwidth = 1;
		c.gridheight = 1;
		c.weighty = .2f;
		add(outputLabel, c);
		c.gridy = 1;
		c.weighty = .7f;
		add(commandLine, c);
		
		addComponentListener(new ComponentAdapter() {
			public void componentResized(ComponentEvent e) {
				adaptLabelFont(outputLabel);
			}
		});
	}

	public void setMinimumDimension() {
		minimumDimension = getSize();
	}

	@SuppressWarnings("deprecation")
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		BufferedImage bg = MainComponent.backgroundImg;

		Util.drawBackground(this, MainComponent.backgroundImg, g);
	}
}
