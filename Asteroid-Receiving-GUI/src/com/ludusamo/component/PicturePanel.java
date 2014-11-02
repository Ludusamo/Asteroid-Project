package com.ludusamo.component;

import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JPanel;

import com.ludusamo.util.Util;

public class PicturePanel extends JPanel {
	private BufferedImage[] images = new BufferedImage[10];
	private int currentImage;

	public PicturePanel(String[] imagesPaths) {
		currentImage = 0;
		
		for (int i = 0; i < imagesPaths.length; i++) {
			try {
				images[i] = ImageIO.read(new File(imagesPaths[i]));
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		setVisible(true);
	}

	public void setPicture(int index) {
		currentImage = index;
	}
	
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		Util.drawBackground(this, MainComponent.backgroundImg, g);
		g.drawImage(images[currentImage], 26, 26, this.getWidth() - 52, this.getHeight() - 52, null);
	}
}
