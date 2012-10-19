package orz.school.compiler;

import java.awt.Color;
import java.awt.EventQueue;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.border.Border;

public class CompilerMainFrame implements ActionListener {

	private JFrame frame;
	private JTextArea jInput;
	private JTextArea jOutToken;
	private JButton btnAnalysis;
	private JButton btnClear;
	private JButton btnRead;
	private JButton btnSave;
	private JTextArea jOutSymbol;
	
	HashMap<String, SymEntry> symboleTable = new HashMap<String, SymEntry>();
	private JScrollPane scrollPane;
	private JScrollPane scrollPane_1;
	private JScrollPane scrollPane_2;
	private JTextArea outPutProduction;
	private JButton btnSytact;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					CompilerMainFrame window = new CompilerMainFrame();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	/**
	 * handle event
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		if(e.getSource() == btnAnalysis) {
			String target = jInput.getText();
			String line = "";
			String result = "";
			int begin = 0;
			int end = 0;
			
			while(true) {
				end = target.indexOf("\n",begin);
				if(end < 0)
				{
					if(begin<target.length())
						result += LexAnalyser.analysis(target.substring(begin, target.length()), symboleTable);
					jOutToken.setText(result);
					break;
				}
				result += LexAnalyser.analysis(target.substring(begin, end), symboleTable);
				begin = end+1;
				jOutToken.setText(result);
			}
			
			String tempstr = "";
			for(String s:symboleTable.keySet()) {
				tempstr+= s+'\t'+symboleTable.get(s).type +'\t' 
						   + symboleTable.get(s).value + '\n';
			}
			jOutSymbol.setText(tempstr);
		}
		else if(e.getSource() == btnSytact) {
			outPutProduction.setText(SyntaxAnalyser.analysis(outPutProduction, "outputToken", symboleTable));
			
			String tempstr = "";
			for(String s:symboleTable.keySet()) {
				tempstr+= s+'\t'+symboleTable.get(s).type +'\t' 
						   + symboleTable.get(s).value + '\n';
			}
			jOutSymbol.setText(tempstr);
		}
		else if(e.getSource() == btnClear) {
			jOutSymbol.setText("");
			jOutToken.setText("");
			jInput.setText("");
			outPutProduction.setText("");
			symboleTable.clear();
		}
		else if(e.getSource() == btnRead) {
			try {
				BufferedReader file = new BufferedReader(new FileReader("input"));
				String tempstr = "";
				String line = null;
				while((line = file.readLine()) != null) {
					tempstr+=line+'\n';
				}
				jInput.setText(tempstr);
				file.close();
			} catch (Exception e1) {
				e1.printStackTrace();
			}
		}
		else if(e.getSource() == btnSave) {
			try {
				BufferedWriter file = new BufferedWriter(new FileWriter("outputToken"));
				file.write(jOutToken.getText());
				file.close();
				
				file = new BufferedWriter(new FileWriter("outputSym"));
				file.write(jOutSymbol.getText());
				file.close();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}
		
	}

	/**
	 * Create the application.
	 */
	public CompilerMainFrame() {
		initialize();
		
		btnAnalysis.addActionListener(this);
		btnClear.addActionListener(this);
		btnRead.addActionListener(this);
		btnSave.addActionListener(this);
		btnSytact.addActionListener(this);
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame();
		frame.setTitle("\u8BCD\u6CD5\u5206\u6790\u5668\u754C\u9762");
		frame.setBounds(100, 100, 953, 511);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(null);
		
		btnSave = new JButton("\u4FDD\u5B58");
		btnSave.setIcon(new ImageIcon(CompilerMainFrame.class.getResource("/javax/swing/plaf/metal/icons/ocean/floppy.gif")));
		btnSave.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
			}
		});
		btnSave.setBounds(20, 362, 113, 36);
		frame.getContentPane().add(btnSave);
		
		btnAnalysis = new JButton("\u8BCD\u6CD5\u5206\u6790");
		btnAnalysis.setBounds(20, 43, 113, 36);
		frame.getContentPane().add(btnAnalysis);
		
		btnClear = new JButton("\u6E05\u7A7A");
		btnClear.setBounds(20, 157, 113, 36);
		frame.getContentPane().add(btnClear);
		
		Border border = BorderFactory
				.createMatteBorder(1, 1, 1, 1, Color.BLACK);
		
		JLabel label = new JLabel("\u8F93\u5165\u8868\u8FBE\u5F0F");
		label.setBounds(227, 18, 70, 15);
		frame.getContentPane().add(label);
		
		JLabel lbltoken = new JLabel("\u8F93\u51FATOKEN\u5E8F\u5217");
		lbltoken.setBounds(226, 109, 95, 15);
		frame.getContentPane().add(lbltoken);

		
		JLabel label_1 = new JLabel("\u7B26\u53F7\u8868");
		label_1.setBounds(227, 279, 56, 15);
		frame.getContentPane().add(label_1);
		
		JLabel lblzx = new JLabel("\u4F5C\u8005\uFF1AZX");
		lblzx.setFont(new Font("ËÎÌו", Font.PLAIN, 14));
		lblzx.setForeground(Color.BLUE);
		lblzx.setBounds(556, 438, 56, 21);
		frame.getContentPane().add(lblzx);
		
		btnRead = new JButton("\u8BFB\u53D6");
		btnRead.setIcon(new ImageIcon(CompilerMainFrame.class.getResource("/javax/swing/plaf/metal/icons/ocean/file.gif")));
		btnRead.setBounds(20, 304, 113, 36);
		frame.getContentPane().add(btnRead);
		
		scrollPane = new JScrollPane();
		scrollPane.setBounds(226, 134, 386, 135);
		frame.getContentPane().add(scrollPane);
		
		jOutToken = new JTextArea();
		scrollPane.setViewportView(jOutToken);
		jOutToken.setBorder(border);
		
		scrollPane_1 = new JScrollPane();
		scrollPane_1.setBounds(227, 43, 385, 60);
		frame.getContentPane().add(scrollPane_1);
		
		jInput = new JTextArea();
		scrollPane_1.setViewportView(jInput);
		jInput.setBorder(border);
		
		scrollPane_2 = new JScrollPane();
		scrollPane_2.setBounds(226, 304, 386, 124);
		frame.getContentPane().add(scrollPane_2);
		
		jOutSymbol = new JTextArea();
		scrollPane_2.setViewportView(jOutSymbol);
		jOutSymbol.setBorder(border);
		
		btnSytact = new JButton("\u8BED\u6CD5\u5206\u6790");
		btnSytact.setBounds(20, 98, 113, 36);
		frame.getContentPane().add(btnSytact);
		
		JScrollPane scrollPane_3 = new JScrollPane();
		scrollPane_3.setBounds(662, 43, 235, 294);
		frame.getContentPane().add(scrollPane_3);
		
		outPutProduction = new JTextArea();
		scrollPane_3.setViewportView(outPutProduction);
		outPutProduction.setBorder(border);
		
		JLabel label_2 = new JLabel("\u5206\u6790\u8FC7\u7A0B");
		label_2.setBounds(662, 18, 70, 15);
		frame.getContentPane().add(label_2);
	}
}
