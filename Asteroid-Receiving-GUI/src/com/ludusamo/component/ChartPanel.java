package com.ludusamo.component;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.imageio.ImageIO;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;

import com.ludusamo.util.Util;
import com.ludusamo.util.Vector2f;

public class ChartPanel extends JPanel {
	JScrollPane scrollPane;
	JTable dataTable;
	DefaultTableModel model;
	JScrollBar bar;

	ArrayList<Vector2f> points;

	BufferedImage title;
	
	Graphics g;

	public ChartPanel(String[] columnNames) {
		scrollPane = new JScrollPane();
		points = new ArrayList<Vector2f>();
		dataTable = new JTable();

		model = new DefaultTableModel();
		dataTable.setModel(model);
		for (int i = 0; i < columnNames.length; i++) {
			model.addColumn(columnNames[i]);
		}
		bar = scrollPane.getVerticalScrollBar();
		scrollPane.setViewportView(dataTable);

		float width = .8f;
		float height = .75f;
		scrollPane.setPreferredSize(new Dimension((int) (getWidth() * width),
				(int) (getHeight() * height)));
		scrollPane.setSize(new Dimension((int) (getWidth() * width),
				(int) (getHeight() * height)));
		
		setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.gridwidth = 1;
		c.gridheight = 1;
		c.gridx = 1;
		c.gridy = 1;
		add(scrollPane);
		
		setVisible(true);
		validate();
		
		try {
			title = ImageIO.read(new File("res/chart_title.png"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void addValue(float d, float mvs) {
		model = (DefaultTableModel) dataTable.getModel();
		model.addRow(new Object[] { d, mvs });
		points.add(new Vector2f(d, mvs));
	}

	public void reset() {
		points.clear();
		model.setRowCount(0);
	}

	public ArrayList<Vector2f> getPoints() {
		return points;
	}

	@SuppressWarnings("deprecation")
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);

		Util.drawBackground(this, MainComponent.backgroundImg, g);
		Util.drawTitle(this, title, g);
	}

	public void resize() {
		float width = .8f;
		float height = .7f;
		scrollPane.setPreferredSize(new Dimension((int) (getWidth() * width),
				(int) (getHeight() * height)));
		scrollPane.setSize(new Dimension((int) (getWidth() * width),
				(int) (getHeight() * height)));
	}
}
