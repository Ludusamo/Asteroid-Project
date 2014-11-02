package com.ludusamo.component;

import java.awt.Color;
import java.awt.Graphics;

import javax.swing.JPanel;

public class ColorPanel extends JPanel {
	private int color;

	public ColorPanel(int color) {
		this.color = color;
		setVisible(true);
	}

	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.setColor(new Color(color));
		g.fillRect(0, 0, getWidth(), getHeight());
	}
}
