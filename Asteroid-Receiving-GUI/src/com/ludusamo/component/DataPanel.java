package com.ludusamo.component;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JLabel;
import javax.swing.JPanel;

import com.ludusamo.util.Util;

public class DataPanel extends JPanel {
	public static final int MIN_FONT_SIZE = 24;
	public static final int MAX_FONT_SIZE = 30;
	private Dimension minimumDimension;

	JLabel[] labels;

	BufferedImage title;
	
	Graphics g;

	private void changeFontSize(JLabel l, int size) {
		Font f = l.getFont();
		l.setFont(f.deriveFont(f.getStyle(), size));
	}

	protected void adaptLabelFont(JLabel l) {
		if (g == null) {
			// System.out.println("Not working.");
			return;
		}

		float newSize = (minimumDimension.width / getSize().width)
				* (MAX_FONT_SIZE - MIN_FONT_SIZE);
		newSize = MAX_FONT_SIZE - newSize;

		changeFontSize(l, (int) newSize);

		l.repaint();
		revalidate();
	}

	public DataPanel() {
		setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		labels = new JLabel[4];
		for (int i = 0; i < labels.length; i++) {
			c.anchor = GridBagConstraints.WEST;
			c.ipadx = 150;
			c.weightx = .7f;
			c.gridwidth = 4;
			c.gridheight = 1;
			c.gridx = 0;
			c.gridy = i + (i * 2);
			labels[i] = new JLabel(" Test");
			labels[i].setFont(MainComponent.font.deriveFont(12));
			labels[i].setForeground(Color.white);
			add(labels[i], c);
		}

		labels[0].setText(" Collecting Data: "
				+ "NaN");
		labels[1].setText(" Time Elapsed: " + 0 + "s");
		labels[2].setText(" Current Slope: " + "NaN");
		labels[3].setText(" Number of Data Points: " + 0);
		
		validate();
		addComponentListener(new ComponentAdapter() {
			public void componentResized(ComponentEvent e) {
				for (int i = 0; i < labels.length; i++) {
					adaptLabelFont(labels[i]);
				}
			}
		});
		
		try {
			title = ImageIO.read(new File("res/data_title.png"));
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}

	public void update(boolean collectingData, float currentSlope,
			int timeElapsed, int numData) {
		labels[0].setText(" Collecting Data: "
				+ (collectingData ? "Yes" : "No"));
		labels[1].setText(" Time Elapsed: " + timeElapsed + "s");
		labels[2].setText(" Current Slope: " + currentSlope);
		labels[3].setText(" Number of Data Points: " + numData);
	}

	public void setMinimumDimension() {
		minimumDimension = getSize();
	}

	@SuppressWarnings("deprecation")
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		this.g = g;

		Util.drawBackground(this, MainComponent.backgroundImg, g);
		Util.drawTitle(this, title, g);
	}
}
