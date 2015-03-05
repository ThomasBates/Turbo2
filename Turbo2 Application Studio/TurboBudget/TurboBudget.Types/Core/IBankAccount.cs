using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TurboBudget.Types
{
    public interface IBankAccount
    {
        int NextTransactionIndex { get; set; }
        string AccountNumber { get; set; }
    }
}
